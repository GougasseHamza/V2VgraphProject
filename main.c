#include "Vehicule.h"
#include "Simulation.h"
#include "graphalgos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(NULL));
    int timeStep = 0;
    int infectedNumber = 0;
    // initialize vehicules array  
    initVehicules();

    // initialize the patient zero 
    Vehicule *p0 = initPatientZero();
    // set the first element of the array to patient zero 
    allVehicules[0] = *p0; 

    // we fill the other slots of the array with vehicules that will be generated 
    // with the initVehicule() function
    for(int i=1; i<MAX_VEHICULES; i++) {
        initVehicule(&allVehicules[i]); 
    }

    // initialize out graph

    initGraph();

    printf("=== V2V SIMULATION ===\n");
    printf("Vehicles: %d | Radius: %d\n", MAX_VEHICULES, DANGER_RADIUS);


    // here we assume that we will run our program for 60 units of time 
    while(timeStep <= 60){ 
        // each step consume 1 unit of time 
        runStep(1.0f); 
        // call infectVehicules() at each delta time because we have to see if there is 
        // any changes that we can do since vehicules are constantly moving 
        infectVehicules(timeStep, &infectedNumber); 

        if(timeStep > 0 && timeStep % 20 == 0) {
            printf("\n\n>>> SNAPSHOT T=%d <<<\n", timeStep);
            printf("Infected Count: %d\n", infectedNumber);

            analyzeClusters();
            computeBackboneMST();
            identifyCriticalVehicles();
            findFastestPath(0, 5); 
        }
        timeStep++;
    } 

    printf("\n=== END ===\n");
    printf("Final Infected: %d\n", infectedNumber);
    free(allVehicules);
    return 0;
}
