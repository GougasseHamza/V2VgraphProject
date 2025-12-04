#include <stdio.h>

#define V 4
#define INF 1e4


void floydWarshall(int dist[][V]) {
    int i, j, k;
    int par[V][V] ; 
    for (k = 0; k < V; k++) {
        for (i = 0; i < V; i++) {
            for (j = 0; j < V; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) 

                { dist[i][j] = dist[i][k] + dist[k][j];
                    par[i][j] = k ; 
                }
            }
        }
    }
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF)
                printf("%s ", "INF");
            else
                printf("%d ", dist[i][j]);
        }
        printf("\n");
    }
}


int main() {
    int graph[V][V] = { 
        {0, 5, INF, 10},
        {INF, 0, 3, INF},
        {INF, INF, 0, 1},
        {INF, INF, INF, 0}
    };

    floydWarshall(graph);

    return 0;
}