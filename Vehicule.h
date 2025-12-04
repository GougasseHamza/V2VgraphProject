#ifndef VEHICULE_H
#define VEHICULE_H

typedef enum {
    STATE_SUSEPTIBLE,
    STATE_INFECTED,
    STATE_RECOVERED
} InfectionState;

typedef struct {
    int id;             
    
    // This field is required for the graph logic
    int originalIndex;  
    
    InfectionState state;
    float position;
    float velocity;
    float infectionProb; 
    int infectionTime;   
} Vehicule;

void initVehicules();
void initVehicule(Vehicule *v);
Vehicule * initPatientZero();

extern Vehicule *allVehicules;

#endif