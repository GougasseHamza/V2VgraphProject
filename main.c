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
    
    initVehicules();
    
    Vehicule *p0 = initPatientZero();
    allVehicules[0] = *p0; 
    
 
    for(int i=1; i<MAX_VEHICULES; i++) {
        initVehicule(&allVehicules[i]); 
    }
    
    initGraph();

    printf("=== V2V SIMULATION ===\n");
    printf("Vehicles: %d | Radius: %d\n", MAX_VEHICULES, DANGER_RADIUS);

    while(timeStep <= 60){ 
        runStep(1.0f); 
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