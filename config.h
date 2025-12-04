#ifndef CONFIG_H
#define CONFIG_H

// Simulation Settings
#define MAX_VEHICULES 50        // Number of vehicles (Scalable up to 1000s)
#define MAX_VELOCITY 60
#define MIN_VELOCITY 40
#define MAX_POSITION 5000       // Length of the road
#define MIN_POSITION 0
#define DANGER_RADIUS 300       // Connection range
#define INFECT_PROB 0.55        // Probability threshold for "Infected" state
#define TRANSMISSION_RATE 0.10f // Risk added per second of contact

// Graph Memory Settings (Static List Optimization)
// Estimate: Avg 20 neighbors per car. 
// If this is too small, edges will simply stop being added (safe).
#define MAX_EDGES (MAX_VEHICULES * 20) 

#define MAX_ID_VALUE 1000

#endif