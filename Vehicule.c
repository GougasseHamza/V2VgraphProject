#include "Vehicule.h"
#include <stdlib.h>
#include "config.h"

// This array will allow us to keep track of all the vehicules that we have on our highway 
Vehicule *allVehicules = NULL;

// initiliaze method : that will initialize the array above 
void initVehicules() {
    allVehicules = malloc(MAX_VEHICULES * sizeof(Vehicule));
}

// this function initializes a vehicule
void initVehicule(Vehicule *v) {
    // generating a random ID for the vehicule
    v->id = rand() % MAX_ID_VALUE;
    
    v->originalIndex = (int)(v - allVehicules);
    
    // position is generated between a minimum and a maximum set in the config.h 
    // MAX_POSITION value and MIN_POSITION value are in that file
    v->position = MIN_POSITION + rand() % (MAX_POSITION - MIN_POSITION + 1);
    // same thing for velocity there is a MIN_VELOCITY and MAX_VELOCITY that are set in the config.h 
    // and the velocity is generated between those two values 
    v->velocity = MIN_VELOCITY + rand() % (MAX_VELOCITY - MIN_VELOCITY + 1);
    
    // each vehicule starts with the STATE_SUSEPTIBLE because when the vehicule is in the road it has a chance 
    // to be infected 
    v->state = STATE_SUSEPTIBLE;
    
    v->infectionProb = 0.0f;
    v->infectionTime = -1;
}

// this function initializes the patient zero, which is the first infected vehicule that will target all 
// the other vehicules 
Vehicule * initPatientZero(){

    Vehicule * patientZero = malloc(sizeof(Vehicule)); 
    
    // we gave ouur vehicule the id 404 
    patientZero->id = 404;
    

    patientZero->originalIndex = 0; 
    
    // obviously its state is STATE_INFECTED
    patientZero->state = STATE_INFECTED;

    // infection prob is 1 because it can infect other vehicules
    patientZero->infectionProb = 1.0f; 
    // let's assume that this patient zero starts in the position MAX_POSITION / 2
    patientZero->position = MAX_POSITION / 2; 
    // the patient zero has a velocity of 50 
    patientZero->velocity = 50;
    
    patientZero->infectionTime = 0;
    
    return patientZero;
}
