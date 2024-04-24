#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

#define V 9
#define NTHREADS 3

int** G;
int* parent;
int* visited;
int* dist;
int minVertex;
int minDistance;

void findMin(int start) {
    #pragma omp parallel for shared(minVertex, minDistance)
    for (int i = start; i < V; i++) {
        if (!visited[i]) {
            #pragma omp critical
            {
                if (dist[i] < minDistance) {
                    minVertex = i;
                    minDistance = dist[i];
                }
            }
        }
    }
}

void updateDist(int start) {
    #pragma omp parallel for
    for (int i = start; i < V; i++) {
        if (!visited[i] && G[minVertex][i] != 0 && dist[minVertex] + G[minVertex][i] < dist[i]) {
            dist[i] = dist[minVertex] + G[minVertex][i];
            parent[i] = minVertex;
        }
    }
}

int main(){
    FILE* fin=fopen("Input.txt","r");
    if(!fin){
        printf("File cannot be read\n");
        exit(EXIT_FAILURE);
    }

    G=malloc(V * sizeof(int*));
    for(int i=0;i<V;i++){
        G[i] = malloc(V * sizeof(int));
        for (int j = 0; j < V; j++)fscanf(fin, "%d", &G[i][j]);
    }

    parent = malloc(V * sizeof(int));
    visited = malloc(V * sizeof(int));
    dist = malloc(V * sizeof(int));

    for (int i = 0; i < V; i++) {
        parent[i] = -1;
        visited[i] = 0;
        dist[i] = INT_MAX;
    }

    dist[0] = 0;

    printf("Edge \tWeight\n");
    for (int k = 0; k < V; k++) {
        minVertex = -1;
        minDistance = INT_MAX;

        #pragma omp parallel sections
        {
            #pragma omp section
            findMin(0);
            #pragma omp section
            {
                #pragma omp barrier
                visited[minVertex] = 1;
            }
            #pragma omp section
            updateDist(0);
        }
    }

    printf("Distance from vertex %d to:\n", 0);
    for (int i = 0; i < V; i++) {
        printf("%d: ", i);
        if (dist[i] != INT_MAX) printf("%d\n", dist[i]);
        else printf("Not reachable\n");
    }

    return 0;
}