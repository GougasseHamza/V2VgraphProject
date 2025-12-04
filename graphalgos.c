#include "graphalgos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>


// creating the structure of node
typedef struct {
    int to;
    float weight;
} Edge;

// creating the data structure Vector that will contain the Egdes to represent our graph
typedef struct {
    Edge* data;     
    int size;      
    int capacity; 
} Vector;

Vector adj[MAX_VEHICULES];

// this function clears our graph it resets it 
void clearGraph() {
    for(int i=0; i<MAX_VEHICULES; i++) {
        adj[i].size = 0;       
    }
}

// function that adds an edge to out graph
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
// implement the dsu as we saw in the course we will use it in some algorithm later 
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

// this function analyzes the graph we have and give us information 
// about the different sets we have and their number
void analyzeClusters() {
    printf("\n\x1b[36m\x1b[1m");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║          GRAPH ANALYSIS: CLUSTERS (DSU)                  ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\x1b[0m");

    for (int i = 0; i < MAX_VEHICULES; i++) make_set(i);


    for (int u = 0; u < MAX_VEHICULES; u++) {
        for (int i = 0; i < adj[u].size; i++) {
            Edge e = adj[u].data[i];
            union_sets(u, e.to);
        }
    }

    int visited[MAX_VEHICULES] = {0};
    int componentCount = 0;

    printf("\n");
    for (int i = 0; i < MAX_VEHICULES; i++) {
        int root = find_set(i);
        if (!visited[root]) {
            componentCount++;
            printf("\x1b[33m▸ Cluster %d\x1b[0m (Root: \x1b[32mID %d\x1b[0m)\n  ",
                   componentCount, allVehicules[root].id);
            printf("Members: { ");
            for (int j = 0; j < MAX_VEHICULES; j++) {
                if (find_set(j) == root) printf("\x1b[36m%d\x1b[0m ", allVehicules[j].id);
            }
            printf("}\n\n");
            visited[root] = 1;
        }
    }
    printf("\x1b[35m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("Total Independent Communities: \x1b[1m%d\x1b[0m\n", componentCount);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m\n");
}

typedef struct {
    int vertex;
    float distance;
} HeapNode;

typedef struct {
    HeapNode data[MAX_VEHICULES];
    int size;
} MinHeap;

void heapInit(MinHeap* heap) {
    heap->size = 0;
}

void heapSwap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->data[idx].distance < heap->data[parent].distance) {
            heapSwap(&heap->data[idx], &heap->data[parent]);
            idx = parent;
        } else {
            break;
        }
    }
}

void heapifyDown(MinHeap* heap, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;

        if (left < heap->size && heap->data[left].distance < heap->data[smallest].distance) {
            smallest = left;
        }
        if (right < heap->size && heap->data[right].distance < heap->data[smallest].distance) {
            smallest = right;
        }

        if (smallest != idx) {
            heapSwap(&heap->data[idx], &heap->data[smallest]);
            idx = smallest;
        } else {
            break;
        }
    }
}

void heapPush(MinHeap* heap, int vertex, float distance) {
    if (heap->size >= MAX_VEHICULES) return;

    heap->data[heap->size].vertex = vertex;
    heap->data[heap->size].distance = distance;
    heapifyUp(heap, heap->size);
    heap->size++;
}

HeapNode heapPop(MinHeap* heap) {
    HeapNode root = heap->data[0];
    heap->size--;
    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        heapifyDown(heap, 0);
    }
    return root;
}

int heapIsEmpty(MinHeap* heap) {
    return heap->size == 0;
}

void findFastestPath(int startIdx, int targetIdx) {
    float dist[MAX_VEHICULES];
    int prev[MAX_VEHICULES];
    int visited[MAX_VEHICULES];
    MinHeap pq;

    heapInit(&pq);

    for (int i = 0; i < MAX_VEHICULES; i++) {
        dist[i] = FLT_MAX;
        visited[i] = 0;
        prev[i] = -1;
    }

    dist[startIdx] = 0;
    heapPush(&pq, startIdx, 0.0f);

    while (!heapIsEmpty(&pq)) {
        HeapNode current = heapPop(&pq);
        int u = current.vertex;

        if (visited[u]) continue;
        if (u == targetIdx) break;

        visited[u] = 1;

        for (int i = 0; i < adj[u].size; i++) {
            Edge e = adj[u].data[i];
            int v = e.to;
            float weight = e.weight;

            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                heapPush(&pq, v, dist[v]);
            }
        }
    }

    printf("\n\x1b[36m\x1b[1m");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║         GRAPH ANALYSIS: OPTIMAL PATH                     ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\x1b[0m");

    printf("\n\x1b[33m➤ Route:\x1b[0m \x1b[32mID %d\x1b[0m → \x1b[32mID %d\x1b[0m\n\n",
           allVehicules[startIdx].id, allVehicules[targetIdx].id);

    if (dist[targetIdx] == FLT_MAX) {
        printf("\x1b[31m✗ No path exists between these vehicles.\x1b[0m\n");
    } else {
        int crawl = targetIdx;
        printf("\x1b[36mPath:\x1b[0m ");
        printf("\x1b[1m%d\x1b[0m", allVehicules[crawl].id);
        while (prev[crawl] != -1) {
            printf(" \x1b[33m←\x1b[0m \x1b[1m%d\x1b[0m", allVehicules[prev[crawl]].id);
            crawl = prev[crawl];
        }
        printf("\n");
        printf("\x1b[35m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("Total Distance: \x1b[1m%.2f\x1b[0m units\n", dist[targetIdx]);
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m\n");
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

    printf("\n\x1b[36m\x1b[1m");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║         GRAPH ANALYSIS: BACKBONE (MST)                   ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\x1b[0m\n");

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

    typedef struct {
        int parent_id;
        int child_id;
    } Link;

    Link links[MAX_VEHICULES];
    int linkCount = 0;

    for (int i = 1; i < MAX_VEHICULES; i++) {
        if (parent[i] != -1) {
            links[linkCount].parent_id = allVehicules[parent[i]].id;
            links[linkCount].child_id = allVehicules[i].id;
            linkCount++;
        }
    }

    if (linkCount > 0) {
        printf("\x1b[36m");
        printf("                   🌳 Network Tree Structure 🌳\n\n");
        printf("\x1b[0m");

        int linksPerRow = 3;
        for (int i = 0; i < linkCount; i++) {
            if (i % linksPerRow == 0) {
                if (i > 0) printf("\n");
                printf("        ");
            }

            printf("\x1b[32m%3d\x1b[0m\x1b[35m━\x1b[36m⬤\x1b[35m━\x1b[0m\x1b[32m%-3d\x1b[0m",
                   links[i].parent_id, links[i].child_id);

            if ((i + 1) % linksPerRow != 0 && i < linkCount - 1) {
                printf("  ");
            }
        }

        printf("\n\n\x1b[35m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("📊 Network Statistics: \x1b[1m%d\x1b[0m\x1b[35m backbone connections established\n", linkCount);
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m\n");
    } else {
        printf("\x1b[31m✗ No MST links found (disconnected graph)\x1b[0m\n");
    }
}

void identifyCriticalVehicles() {
    printf("\n\x1b[36m\x1b[1m");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║         GRAPH ANALYSIS: CRITICAL HUBS                    ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\x1b[0m\n");

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
        printf("\x1b[33m★ Critical Hub Identified:\x1b[0m\n");
        printf("  \x1b[32mVehicle ID:\x1b[0m \x1b[1m%d\x1b[0m\n", allVehicules[hubIndex].id);
        printf("  \x1b[32mConnections:\x1b[0m \x1b[1m%d\x1b[0m\n", maxDegree);
        printf("\n\x1b[35m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("This vehicle is the most connected hub in the network.\n");
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\x1b[0m\n");
    } else {
        printf("\x1b[31m✗ No critical hub found.\x1b[0m\n");
    }
}
