#include "graphalgos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>



typedef struct {
    int to;
    float weight;
} Edge;

// Dynamic Array Structure
typedef struct {
    Edge* data;     // Pointer to the array on heap
    int size;       // Current number of elements
    int capacity;   // Max elements before realloc needed
} Vector;

Vector adj[MAX_VEHICULES];


void clearGraph() {

    for(int i=0; i<MAX_VEHICULES; i++) {
        adj[i].size = 0;       
    }
}

void addEdge(int u, int v, float weight) {
    Vector* vec = &adj[u];

    if (vec->data == NULL) {
        vec->capacity = 16; // Start with small capacity
        vec->data = (Edge*)malloc(vec->capacity * sizeof(Edge));
        vec->size = 0;
    }

    // B. Resize if full 
    if (vec->size == vec->capacity) {
        vec->capacity *= 2;
        vec->data = (Edge*)realloc(vec->data, vec->capacity * sizeof(Edge));
    }

    vec->data[vec->size].to = v;
    vec->data[vec->size].weight = weight;
    vec->size++;
}

//dsu 

int parent[MAX_VEHICULES];
int rank[MAX_VEHICULES];

void make_set(int v) {
    parent[v] = v;
    rank[v] = 0;
}

int find_set(int v) {
    if (v == parent[v]) return v;
    return parent[v] = find_set(parent[v]);
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (rank[a] < rank[b]) parent[a] = b;
        else {
            parent[b] = a;
            if (rank[a] == rank[b]) rank[a]++;
        }
    }
}


void analyzeClusters() {
    printf("\n--- GRAPH ANALYSIS: CLUSTERS (DSU) ---\n");

    for (int i = 0; i < MAX_VEHICULES; i++) make_set(i);

    
    for (int u = 0; u < MAX_VEHICULES; u++) {
        for (int i = 0; i < adj[u].size; i++) {
            Edge e = adj[u].data[i];
            union_sets(u, e.to);
        }
    }

    int visited[MAX_VEHICULES] = {0}; 
    int componentCount = 0;

    for (int i = 0; i < MAX_VEHICULES; i++) {
        int root = find_set(i);
        if (!visited[root]) {
            componentCount++;
            printf("Cluster %d (Root ID %d): { ", componentCount, allVehicules[root].id);
            for (int j = 0; j < MAX_VEHICULES; j++) {
                if (find_set(j) == root) printf("%d ", allVehicules[j].id);
            }
            printf("}\n");
            visited[root] = 1;
        }
    }
    printf("Total Independent Communities: %d\n", componentCount);
}

void findFastestPath(int startIdx, int targetIdx) {
    float dist[MAX_VEHICULES];
    int prev[MAX_VEHICULES];
    int visited[MAX_VEHICULES];

    for (int i = 0; i < MAX_VEHICULES; i++) {
        dist[i] = FLT_MAX;
        visited[i] = 0;
        prev[i] = -1;
    }

    dist[startIdx] = 0;

    for (int count = 0; count < MAX_VEHICULES - 1; count++) {
        float min = FLT_MAX;
        int u = -1;
        
        for (int v = 0; v < MAX_VEHICULES; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }

        if (u == -1 || u == targetIdx) break;
        visited[u] = 1;

        for (int i = 0; i < adj[u].size; i++) {
            Edge e = adj[u].data[i];
            int v = e.to;
            float weight = e.weight;
            
            if (!visited[v] && dist[u] != FLT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
            }
        }
    }

    printf("\n--- GRAPH ANALYSIS: OPTIMAL PATH (ID %d -> ID %d) ---\n", 
           allVehicules[startIdx].id, allVehicules[targetIdx].id);
    
    if (dist[targetIdx] == FLT_MAX) {
        printf("No path exists.\n");
    } else {
        int crawl = targetIdx;
        printf("Path: %d", allVehicules[crawl].id);
        while (prev[crawl] != -1) {
            printf(" <- %d", allVehicules[prev[crawl]].id);
            crawl = prev[crawl];
        }
        printf("\nTotal Weight: %.2f\n", dist[targetIdx]);
    }
}



//mst 
void computeBackboneMST() {
    int parent[MAX_VEHICULES]; 
    float key[MAX_VEHICULES];   
    int mstSet[MAX_VEHICULES];  

    for (int i = 0; i < MAX_VEHICULES; i++) {
        key[i] = FLT_MAX;
        mstSet[i] = 0;
        parent[i] = -1; 
    }

    key[0] = 0.0f; 
    parent[0] = -1; 

    printf("\n--- GRAPH ANALYSIS: BACKBONE (MST) ---\n");

    for (int count = 0; count < MAX_VEHICULES - 1; count++) {
        float min = FLT_MAX;
        int u = -1;
        for (int v = 0; v < MAX_VEHICULES; v++) {
            if (mstSet[v] == 0 && key[v] < min) {
                min = key[v];
                u = v;
            }
        }

        if (u == -1) break;
        mstSet[u] = 1;

        for (int i = 0; i < adj[u].size; i++) {
            Edge e = adj[u].data[i];
            int v = e.to;
            float weight = e.weight;
            
            if (mstSet[v] == 0 && weight < key[v]) {
                parent[v] = u;
                key[v] = weight;
            }
        }
    }

    for (int i = 1; i < MAX_VEHICULES; i++) {
        if (parent[i] != -1) {
            printf("Backbone Link: ID %d <---> ID %d\n", 
                   allVehicules[parent[i]].id, allVehicules[i].id);
        }
    }
}

void identifyCriticalVehicles() {
    printf("\n--- GRAPH ANALYSIS: CRITICAL HUBS ---\n");
    int maxDegree = -1;
    int hubIndex = -1;

    for(int i=0; i<MAX_VEHICULES; i++) {
        // Degree is simply the vector size
        int degree = adj[i].size;
        
        if(degree > maxDegree) {
            maxDegree = degree;
            hubIndex = i;
        }
    }

    if(hubIndex != -1) {
        printf("Critical Node: ID %d (Connections: %d)\n", 
               allVehicules[hubIndex].id, maxDegree);
    }
}