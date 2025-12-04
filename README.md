# V2V Malware Propagation Simulator

## Overview

This project simulates malware propagation in Vehicle-to-Vehicle (V2V) communication networks using graph theory and epidemic models. Vehicles are represented as nodes in a dynamic graph, with proximity-based communication links as edges.

## Project Structure

```
.
├── main.c              # Main simulation loop and program entry point
├── Vehicule.h/c        # Vehicle data structure and initialization
├── Simulation.h/c      # Simulation engine and infection logic
├── graphalgos.h/c      # Graph algorithms (Dijkstra, Prim, DSU, Centrality)
├── config.h            # Simulation parameters and constants
├── floydd.c            # Floyd-Warshall algorithm (standalone test)
├── PROJECT_REPORT.md   # Comprehensive project report
└── README.md           # This file
```

## Features

### Graph Algorithms Implemented

1. **Cluster Detection (DSU/Union-Find)** - Identifies isolated vehicle communities
2. **Shortest Path (Dijkstra)** - Finds fastest propagation routes
3. **Minimum Spanning Tree (Prim)** - Identifies backbone communication structure
4. **Degree Centrality** - Identifies critical hub vehicles

### Simulation Features

- Dynamic graph topology (vehicles move over time)
- SIR-like epidemic model (Susceptible → Infected → Recovered)
- Proximity-based infection (300-unit communication range)
- Patient Zero initialization (Vehicle ID 404)
- Real-time infection tracking and reporting

## Compilation and Execution

### Requirements
- GCC compiler
- Math library (-lm flag)

### Compile
```bash
gcc -o simulation main.c Vehicule.c Simulation.c graphalgos.c -lm
```

### Run
```bash
./simulation
```

## Configuration

Edit `config.h` to modify simulation parameters:

```c
#define MAX_VEHICULES 50        // Number of vehicles
#define DANGER_RADIUS 300       // Communication range
#define TRANSMISSION_RATE 0.10f // Infection probability per contact
#define MAX_POSITION 5000       // Road length
#define MAX_VELOCITY 60         // Maximum vehicle speed
#define MIN_VELOCITY 40         // Minimum vehicle speed
```

## Output Format

The simulation produces:

1. **Real-time infection events**: Shows when and where vehicles get infected
2. **Periodic snapshots (T=20, T=40, T=60)**:
   - Cluster analysis (which vehicles form connected groups)
   - Backbone MST (critical communication links)
   - Critical hubs (vehicles with most connections)
   - Optimal propagation paths (shortest path from Patient Zero)

### Sample Output

```
=== V2V SIMULATION ===
Vehicles: 50 | Radius: 300

[T=9] INFECTION: ID 170 -> ID 170 (Pos: 2903)
[T=10] INFECTION: ID 745 -> ID 745 (Pos: 3118)

>>> SNAPSHOT T=20 <<<
Infected Count: 19

--- GRAPH ANALYSIS: CLUSTERS (DSU) ---
Total Independent Communities: 2

--- GRAPH ANALYSIS: BACKBONE (MST) ---
Backbone Link: ID 404 <---> ID 745

--- GRAPH ANALYSIS: CRITICAL HUBS ---
Critical Node: ID 523 (Connections: 10)

--- GRAPH ANALYSIS: OPTIMAL PATH (ID 404 -> ID 933) ---
Path: 933 <- 773 <- 370 <- 27 <- 618 <- 851 <- 982 <- 573 <- 404
Total Weight: 1876.00
```

## Algorithm Details

### 1. Cluster Detection (Union-Find)
- **Purpose**: Identify isolated groups of vehicles
- **Complexity**: O(E·α(V)) ≈ O(E) with path compression
- **Implementation**: Rank-based union with path compression

### 2. Shortest Path (Dijkstra)
- **Purpose**: Find fastest propagation route to target
- **Complexity**: O(V²)
- **Implementation**: Array-based min-heap

### 3. Minimum Spanning Tree (Prim)
- **Purpose**: Identify backbone communication structure
- **Complexity**: O(V²)
- **Implementation**: Key-based MST construction

### 4. Degree Centrality
- **Purpose**: Identify super-spreader vehicles
- **Complexity**: O(V)
- **Implementation**: Simple edge counting

## Attack Scenario

**Malware**: "Phantom Tracker"
- Initial infection via compromised mobile app
- Exploits DSRC protocol vulnerabilities
- Spreads through V2V beacons
- Harvests GPS data and authentication tokens

**Timeline**:
- T=0: Patient Zero (ID 404) infected
- T=20: 38% of fleet infected (19/50)
- T=40: 90% of fleet infected (45/50)
- T=60: Near-complete network compromise

## Project Report

See `PROJECT_REPORT.md` for the complete technical report including:
- Graph modeling justification
- Detailed attack scenario
- Algorithm justifications
- Research references
- Sample input/output analysis

## Authors

[Your Names Here]

## Course Information

**Course**: Graph Theory and Applications
**Institution**: Université Mohammed VI Polytechnique
**Submission Date**: December 4, 2024

## Acknowledgments

- Graph algorithms based on standard implementations from Cormen et al., "Introduction to Algorithms"
- Epidemic modeling inspired by Keeling & Eames (2005)
- V2V security concepts from Raya & Hubaux (2007)
