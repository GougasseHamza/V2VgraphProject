#include "Vehicule.h"
#include <stdlib.h>
#include "config.h"

Vehicule *allVehicules = NULL;

void initVehicules() {
    allVehicules = malloc(MAX_VEHICULES * sizeof(Vehicule));
}

void initVehicule(Vehicule *v) {
    v->id = rand() % MAX_ID_VALUE;
    
    v->originalIndex = (int)(v - allVehicules);
    
    v->position = MIN_POSITION + rand() % (MAX_POSITION - MIN_POSITION + 1);
    v->velocity = MIN_VELOCITY + rand() % (MAX_VELOCITY - MIN_VELOCITY + 1);
    
    v->state = STATE_SUSEPTIBLE;
    v->infectionProb = 0.0f;
    v->infectionTime = -1;
}

Vehicule * initPatientZero(){
    Vehicule * patientZero = malloc(sizeof(Vehicule)); 
    patientZero->id = 404;
    
    patientZero->originalIndex = 0; 
    
    patientZero->state = STATE_INFECTED;
    patientZero->infectionProb = 1.0f; 
    patientZero->position = MAX_POSITION / 2; 
    patientZero->velocity = 50;
    
    patientZero->infectionTime = 0;
    
    return patientZero;
}