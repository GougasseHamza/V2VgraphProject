#include "Simulation.h"
#include "Vehicule.h"
#include "config.h"
#include "graphalgos.h" 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float nextStepProb[MAX_VEHICULES];
int infectionSource[MAX_VEHICULES]; 

// this function will initialze our graph 
void initGraph(){
    clearGraph();
}



//custom comparator based on position 
int compareVehicles(const void *a, const void *b) {
    Vehicule **v1 = (Vehicule **)a;
    Vehicule **v2 = (Vehicule **)b;
    if ((*v1)->position < (*v2)->position) return -1;
    if ((*v1)->position > (*v2)->position) return 1;
    return 0;
}


//binary search to find the last index where the node can reach it 
int getUpperBound(Vehicule **sortedPtrs, int start, int end, float limit) {
    int left = start;
    int right = end;
    int ans = start - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (sortedPtrs[mid]->position <= limit) {
            ans = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return ans;
}


// this runStep is the function that will our scenario move 
// so it takes as a parameter a short period of time dt 
// it updated the position of all the vehicules according to their velocity 
void runStep(float dt){
    for(int i=0; i<MAX_VEHICULES; i++){
       
        allVehicules[i].position += dt * allVehicules[i].velocity;

        nextStepProb[i] = allVehicules[i].infectionProb;
    }
}

// this function will help us to print an infection 
// just to make the code clear and a readable output 
void printInfection(int time, Vehicule infected, Vehicule source){
    printf("\x1b[31m🦠 [T=%d]\x1b[0m Infection: \x1b[33mID %d\x1b[0m → \x1b[31mID %d\x1b[0m (Position: \x1b[36m%.0f\x1b[0m)\n",
           time, source.id, infected.id, infected.position);
}


// this function will
void infectVehicules(int time, int *infectedVehiculesNumber){
    clearGraph();
    //the graph changes at each time , because if i and j are tied today they might not be after delta t or two delta t
    Vehicule* sortedPtrs[MAX_VEHICULES];
    for(int i=0; i<MAX_VEHICULES; i++) sortedPtrs[i] = &allVehicules[i];
    qsort(sortedPtrs, MAX_VEHICULES, sizeof(Vehicule*), compareVehicles);

    for(int i=0; i<MAX_VEHICULES; i++){
        infectionSource[i] = -1;
    }

    // we will loop over each vehicule in the list
    for(int i = 0; i < MAX_VEHICULES; i++){

        Vehicule *v1 = sortedPtrs[i];
        // compute its limit distance that can affect so that we don't loop over vehicule that the current one can't reach
        float limit = v1->position + DANGER_RADIUS;
        // getUpperBound will return the last index that can be reached within this limit
        int lastIdx = getUpperBound(sortedPtrs, i + 1, MAX_VEHICULES - 1, limit);

        // we loop over this range of vehicules
        for(int j = i + 1; j <= lastIdx; j++){
            Vehicule *v2 = sortedPtrs[j];
            // compute the distance between the current one and the ones in the radius
            float dist = v2->position - v1->position;

            // update our edges in the graph
            addEdge(v1->originalIndex, v2->originalIndex, dist);
            addEdge(v2->originalIndex, v1->originalIndex, dist);

            // compute the risk that that v1 infect v2 and also v2 infect v1
            float riskToV1 = v2->infectionProb * TRANSMISSION_RATE;
            float riskToV2 = v1->infectionProb * TRANSMISSION_RATE;

            // we update the nextStepProb array of the two vehicules
            nextStepProb[v1->originalIndex] = 1.0f - (1.0f - nextStepProb[v1->originalIndex]) * (1.0f - riskToV1);
            nextStepProb[v2->originalIndex] = 1.0f - (1.0f - nextStepProb[v2->originalIndex]) * (1.0f - riskToV2);

            if(v2->state == STATE_INFECTED && v1->state != STATE_INFECTED && riskToV1 > 0){
                infectionSource[v1->originalIndex] = v2->originalIndex;
            }
            if(v1->state == STATE_INFECTED && v2->state != STATE_INFECTED && riskToV2 > 0){
                infectionSource[v2->originalIndex] = v1->originalIndex;
            }
        }
    }
    // initialize a counter for the infected vehicules number  
    *infectedVehiculesNumber = 0;
    // loop over vehicules array
    for(int i=0; i<MAX_VEHICULES; i++){
        // update the infectionProb of each vehicule according to the infection probability array
        allVehicules[i].infectionProb = nextStepProb[i];
        
        // here we check if the conditions are satisfied to perform the infection
        // condition 1 : the infectionProb higher than 0.99
        // condition 2 : the target vehicule shouldn't be already infected
        if(allVehicules[i].infectionProb > 0.99f && allVehicules[i].state != STATE_INFECTED){
            // we update the state of the target vehicule
            allVehicules[i].state = STATE_INFECTED;
            // and we set the time of infection
            allVehicules[i].infectionTime = time;
            // we print the infection in the console
            if(infectionSource[i] != -1){
                printInfection(time, allVehicules[i], allVehicules[infectionSource[i]]);
            }
        }
        // we increment the counter of infected vehicule if the condition above is satisfied
        if(allVehicules[i].state == STATE_INFECTED){
            (*infectedVehiculesNumber)++;
        }
    }
}
