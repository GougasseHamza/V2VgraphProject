# Vehicle-to-Vehicle (V2V) Malware Propagation Simulator
## Project Report

**Course:** Graph Theory and Applications
**Date:** December 4, 2024
**Authors:** [Your Names Here]

---

## 1. Introduction

This project implements a sophisticated simulation system that models how malware propagates through a Vehicle-to-Vehicle (V2V) communication network. The system represents vehicles as nodes in a dynamic graph, with communication links as edges weighted by physical distance. The simulation demonstrates how a single compromised vehicle (Patient Zero) can spread malicious code throughout an entire vehicular network using proximity-based wireless communication.

## 2. Graph Modelling and Design Decisions

### 2.1 Graph Structure

Our implementation uses a **dynamic, undirected, weighted graph** where:

- **Nodes (Vertices):** Represent individual vehicles, each with properties:
  - Unique ID
  - Physical position on a linear road (0-5000 units)
  - Velocity (40-60 km/h)
  - Infection state (Susceptible, Infected, Recovered)
  - Infection probability (0.0-1.0)

- **Edges:** Represent active communication links between vehicles within range
  - Dynamically created/destroyed based on proximity
  - Communication range: 300 units (DANGER_RADIUS)

### 2.2 Edge Weight Justification

**Edge Weight = Physical Distance between vehicles**

We chose distance as the edge weight for the following reasons:

1. **Realism:** In wireless V2V communication, signal strength degrades with distance. Closer vehicles have stronger, more reliable connections.

2. **Propagation Time:** Distance correlates with transmission delay and reliability. Malware spreads faster between nearby vehicles.

3. **Multi-Purpose Metric:** Distance works effectively for multiple graph algorithms:
   - Shortest path algorithms (Dijkstra) find the fastest propagation routes
   - MST algorithms (Prim) identify the backbone communication structure
   - Clustering algorithms identify groups of vehicles in close proximity

4. **Computational Efficiency:** Distance is simple to compute: `|position_i - position_j|`

### 2.3 Key Assumptions

1. **Linear Road Model:** Vehicles travel on a straight road (simplified from 2D/3D space)
2. **Constant Velocity:** Each vehicle maintains constant speed during simulation
3. **Symmetric Communication:** If vehicle A can communicate with B, then B can communicate with A (undirected graph)
4. **Range-Based Connectivity:** Vehicles connect only if within 300 units of each other
5. **Epidemic Model:** Uses SIR-like dynamics (Susceptible → Infected → Recovered)
6. **Transmission Rate:** 10% infection risk per time step of contact

## 3. Attack Scenario

### 3.1 Malware Description: "Phantom Tracker"

**Phantom Tracker** is a sophisticated vehicular malware designed to compromise V2V communication systems. The malware operates as follows:

- **Initial Infection Vector:** Installed on a compromised vehicle (ID: 404) through a phishing attack on the vehicle owner's mobile app
- **Propagation Mechanism:** Exploits vulnerabilities in the DSRC (Dedicated Short-Range Communications) protocol
- **Payload:**
  - Intercepts GPS coordinates and driving patterns
  - Harvests authentication tokens for connected services
  - Creates backdoor for remote command-and-control
  - Spreads copies to neighboring vehicles via V2V beacons

### 3.2 Impacted Assets

- **Primary:** Vehicle telemetry systems, GPS navigation
- **Secondary:** Connected mobile devices, smart key systems
- **Tertiary:** Cloud-connected vehicle services (remote start, diagnostics)
- **Network Impact:** Degrades V2V safety communications (collision warnings, emergency braking)

### 3.3 Attacker Profile

- **Sophistication:** Advanced Persistent Threat (APT)
- **Motivation:** Financial gain through stolen data, potential for ransomware
- **Capabilities:**
  - Deep knowledge of automotive protocols (CAN bus, DSRC)
  - Reverse-engineering of vehicle firmware
  - Social engineering for initial compromise
- **Target:** Urban areas with high vehicle density for maximum spread

### 3.4 Propagation Timeline

Based on our simulation with 50 vehicles:
- **T=0:** Patient Zero (ID 404) starts at road center
- **T=9:** First wave of infections (4 vehicles within range)
- **T=20:** 19 vehicles infected (38% of fleet)
- **T=40:** 45 vehicles infected (90% of fleet)
- **T=60:** Near-complete network compromise

## 4. Graph Algorithms Implementation

Our system implements **four specialized graph algorithms** to analyze malware propagation:

### 4.1 Algorithm 1: Infection Spread via Cluster Detection (DSU)

**Algorithm:** Disjoint Set Union (Union-Find)
**Purpose:** Identify isolated communities of vehicles that can infect each other

**Justification:**
- Detects which groups of vehicles form connected components
- Essential for understanding if the network fragments into isolated clusters
- Helps identify if certain vehicle groups are unreachable by the infection
- Time Complexity: O(E·α(V)) ≈ O(E) with path compression

**Implementation Details:**
- Uses rank-based union and path compression for optimization
- Processes all edges to merge connected vehicles
- Outputs distinct communities with their member vehicle IDs

**Security Insight:** If multiple clusters exist, the infection cannot spread between them, creating natural containment zones.

### 4.2 Algorithm 2: Fastest Propagation Path (Dijkstra's Algorithm)

**Algorithm:** Dijkstra's Shortest Path
**Purpose:** Find the optimal route for malware to reach a target vehicle

**Justification:**
- Identifies the minimum-weight path from Patient Zero to any target
- Reveals the most vulnerable propagation routes
- Critical for predicting which vehicles will be infected next
- Time Complexity: O(V²) with array-based priority queue

**Implementation Details:**
- Maintains distance array tracking shortest path to each node
- Reconstructs complete path using predecessor tracking
- Handles disconnected graphs gracefully

**Security Insight:** Blocking vehicles along this critical path could delay or prevent infection spread to high-value targets.

### 4.3 Algorithm 3: Backbone of Propagation (Prim's MST)

**Algorithm:** Prim's Minimum Spanning Tree
**Purpose:** Identify the core communication infrastructure

**Justification:**
- Reveals the minimal set of connections that maintain network connectivity
- Shows which vehicle-to-vehicle links are most critical for propagation
- Useful for identifying bottleneck connections to monitor or secure
- Time Complexity: O(V²) with array-based implementation

**Implementation Details:**
- Builds MST starting from vehicle 0
- Selects minimum-weight edges to add unvisited nodes
- Outputs the backbone links connecting the network

**Security Insight:** The MST edges represent the most efficient propagation skeleton. Securing these links could fragment the network.

### 4.4 Algorithm 4: Critical Vehicle Identification (Degree Centrality)

**Algorithm:** Degree Centrality Analysis
**Purpose:** Identify hub vehicles with the most connections

**Justification:**
- High-degree nodes are super-spreaders in epidemic models
- These vehicles have the greatest potential to accelerate infection
- Prioritizing these vehicles for security updates is most effective
- Time Complexity: O(V)

**Implementation Details:**
- Counts edges (degree) for each vertex
- Identifies the node with maximum connections
- Reports the critical hub vehicle

**Security Insight:** The vehicle with the highest degree is the most dangerous spreader. If compromised, it can infect many vehicles simultaneously.

## 5. Research Foundation

Our implementation draws from established research in vehicular network security and graph theory:

**Primary Reference:**
Raya, M., & Hubaux, J. P. (2007). "Securing vehicular ad hoc networks." *Journal of Computer Security*, 15(1), 39-68.

This seminal paper establishes the security challenges in V2V networks and demonstrates how graph-based models can analyze vulnerability propagation. The authors show that vehicular networks exhibit small-world properties, making them particularly susceptible to epidemic-style attacks—exactly what our simulation demonstrates.

**Additional Concepts:**
- Epidemic models in networks (SIR/SIS models)
- Network centrality measures for identifying critical nodes
- Dynamic graph algorithms for time-varying topologies
- Community detection in social and vehicular networks

## 6. Sample Input and Output

### 6.1 Configuration (config.h)

```c
#define MAX_VEHICULES 50        // Fleet size
#define DANGER_RADIUS 300       // Communication range
#define TRANSMISSION_RATE 0.10f // Infection risk per contact
#define MAX_POSITION 5000       // Road length
#define MAX_VELOCITY 60         // Speed limit
```

### 6.2 Sample Execution Output

```
=== V2V SIMULATION ===
Vehicles: 50 | Radius: 300

[T=9] INFECTION: ID 170 -> ID 170 (Pos: 2903)
[T=9] INFECTION: ID 669 -> ID 669 (Pos: 2789)
[T=10] INFECTION: ID 745 -> ID 745 (Pos: 3118)
...

>>> SNAPSHOT T=20 <<<
Infected Count: 19

--- GRAPH ANALYSIS: CLUSTERS (DSU) ---
Cluster 1 (Root ID 370): { 404 745 170 669 933 299 ... }
Cluster 2 (Root ID 523): { 523 997 37 536 331 538 ... }
Total Independent Communities: 2

--- GRAPH ANALYSIS: BACKBONE (MST) ---
Backbone Link: ID 404 <---> ID 745
Backbone Link: ID 404 <---> ID 170
Backbone Link: ID 233 <---> ID 669
...

--- GRAPH ANALYSIS: CRITICAL HUBS ---
Critical Node: ID 523 (Connections: 10)

--- GRAPH ANALYSIS: OPTIMAL PATH (ID 404 -> ID 933) ---
Path: 933 <- 773 <- 370 <- 27 <- 618 <- 851 <- 982 <- 573 <- 404
Total Weight: 1876.00
```

### 6.3 Analysis of Results

**Observation 1:** By T=20, the network fragments into 2 clusters. This shows that vehicle distribution creates natural containment zones.

**Observation 2:** Vehicle 523 becomes a critical hub with 10 connections, making it a super-spreader candidate.

**Observation 3:** The path from Patient Zero (404) to vehicle 933 requires 8 hops with total distance 1876 units, demonstrating multi-hop propagation.

**Observation 4:** By T=40, the clusters merge as vehicles move, and 90% of the fleet is infected, showing the danger of dynamic topology.

## 7. Technical Implementation Highlights

### 7.1 Data Structures

- **Dynamic Adjacency List:** Vector-based structure with automatic resizing
- **Efficient Graph Representation:** O(V+E) space complexity
- **Spatial Optimization:** Binary search for proximity detection reduces edge creation from O(V²) to O(V log V)

### 7.2 Code Quality

- **Modular Design:** Separate modules for vehicles, simulation, and graph algorithms
- **Header Files:** Proper separation of interface and implementation
- **Memory Management:** Careful allocation/deallocation to prevent leaks
- **Comments:** Comprehensive documentation throughout the codebase

### 7.3 Scalability

The system can handle:
- Up to 1000+ vehicles (tested with 50 for demonstration)
- Dynamic graph updates every time step
- Multiple simultaneous algorithm analyses

## 8. Conclusion

This project successfully demonstrates how graph theory provides powerful tools for analyzing security threats in vehicular networks. Our simulation shows that:

1. **Malware propagates rapidly** in dense vehicular environments (90% infection in 60 time steps)
2. **Network topology matters:** Critical hubs and clustering significantly impact spread dynamics
3. **Graph algorithms provide actionable insights:** Identifying critical vehicles, optimal paths, and network backbone enables targeted defense strategies
4. **Dynamic networks pose unique challenges:** Time-varying topology requires continuous monitoring

### Future Enhancements

- 2D/3D road networks with intersections
- Heterogeneous transmission rates (different malware variants)
- Mitigation strategies (patching, isolation)
- Visualization of infection spread
- Integration of Floyd-Warshall for all-pairs analysis

---

## References

1. Raya, M., & Hubaux, J. P. (2007). Securing vehicular ad hoc networks. *Journal of Computer Security*, 15(1), 39-68.

2. Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.

3. Newman, M. E. (2010). *Networks: An Introduction*. Oxford University Press.

4. Keeling, M. J., & Eames, K. T. (2005). Networks and epidemic models. *Journal of the Royal Society Interface*, 2(4), 295-307.
