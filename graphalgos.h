#ifndef GRAPH_ALGO_H
#define GRAPH_ALGO_H

#include "Vehicule.h"
#include "config.h"

// --- Graph Management (Static List) ---
void clearGraph();
void addEdge(int u, int v, float weight);

// --- Analysis Algorithms ---

// 1. Clusters (Using Disjoint Set Union - DSU)
void analyzeClusters();

// 2. Backbone (Prim's Minimum Spanning Tree)
void computeBackboneMST();

// 3. Fastest Path (Dijkstra's Algorithm)
void findFastestPath(int startNodeIndex, int targetNodeIndex);

// 4. Critical Vehicles (Degree Centrality)
void identifyCriticalVehicles();

#endif