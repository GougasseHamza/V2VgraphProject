#ifndef SIMULATION_H
#define SIMULATION_H
#include "Vehicule.h"
#include "config.h"

void runStep(float dt);
void infectVehicules(int time, int *n); 
void initGraph(); // Calls clearGraph internaly
void printInfection(int time, Vehicule infected, Vehicule source);

#endif