#include "Simulation.h"
#include "Vehicule.h"
#include "config.h"
#include "graphalgos.h" 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float nextStepProb[MAX_VEHICULES]; 

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



void runStep(float dt){
    for(int i=0; i<MAX_VEHICULES; i++){
       
        allVehicules[i].position += dt * allVehicules[i].velocity;

        nextStepProb[i] = allVehicules[i].infectionProb;
    }
}

void printInfection(int time, Vehicule infected, Vehicule source){
    printf("[T=%d] INFECTION: ID %d -> ID %d (Pos: %.0f)\n", 
           time, source.id, infected.id, infected.position);
}

void infectVehicules(int time, int *infectedVehiculesNumber){
    clearGraph();
    //the graph changes at each tie , because if i and j are tied today they mught not be after delta t or two delta t 
    Vehicule* sortedPtrs[MAX_VEHICULES];
    for(int i=0; i<MAX_VEHICULES; i++) sortedPtrs[i] = &allVehicules[i];
    qsort(sortedPtrs, MAX_VEHICULES, sizeof(Vehicule*), compareVehicles);

    for(int i = 0; i < MAX_VEHICULES; i++){
        Vehicule *v1 = sortedPtrs[i];
        
        float limit = v1->position + DANGER_RADIUS;
        int lastIdx = getUpperBound(sortedPtrs, i + 1, MAX_VEHICULES - 1, limit);
        
        for(int j = i + 1; j <= lastIdx; j++){
            Vehicule *v2 = sortedPtrs[j];
            float dist = v2->position - v1->position;

         
            addEdge(v1->originalIndex, v2->originalIndex, dist);
            addEdge(v2->originalIndex, v1->originalIndex, dist);

            float riskToV1 = v2->infectionProb * TRANSMISSION_RATE;
            float riskToV2 = v1->infectionProb * TRANSMISSION_RATE;

            nextStepProb[v1->originalIndex] = 1.0f - (1.0f - nextStepProb[v1->originalIndex]) * (1.0f - riskToV1);
            nextStepProb[v2->originalIndex] = 1.0f - (1.0f - nextStepProb[v2->originalIndex]) * (1.0f - riskToV2);
        }
    }

    *infectedVehiculesNumber = 0;
    for(int i=0; i<MAX_VEHICULES; i++){
        allVehicules[i].infectionProb = nextStepProb[i];
        
        if(allVehicules[i].infectionProb > 0.99f && allVehicules[i].state != STATE_INFECTED){
            allVehicules[i].state = STATE_INFECTED;
            allVehicules[i].infectionTime = time;
            printInfection(time, allVehicules[i], allVehicules[i]); 
        }

        if(allVehicules[i].state == STATE_INFECTED){
            (*infectedVehiculesNumber)++;
        }
    }
}