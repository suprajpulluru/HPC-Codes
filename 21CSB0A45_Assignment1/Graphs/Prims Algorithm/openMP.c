#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>

#define V 9

int **G;
int *vis;
int minDist;
int minVertex;
int *dist;
int *parent;

void prims()
{
    printf("Edges in MST : \n");
    for (int i = 0; i < V; i++)
    {
        minVertex = -1;
        minDist = INT_MAX;

#pragma omp parallel for shared(minVertex, minDist) schedule(static) num_threads(omp_get_max_threads())
        for (int j = 0; j < V; j++)
        {
            if (vis[j] == 1)
                continue;

#pragma omp critical
            {
                if (dist[j] < minDist)
                {
                    minVertex = j;
                    minDist = dist[j];
                }
            }
        }

        if (minVertex != 0)
        {
            printf("%d %d %d \n", parent[minVertex], minVertex, G[minVertex][parent[minVertex]]);
        }
        vis[minVertex] = 1;

#pragma omp parallel for shared(minVertex) schedule(static) num_threads(omp_get_max_threads())
        for (int j = 0; j < V; j++)
        {
            if (vis[j] == 1)
                continue;
            if (G[minVertex][j] != 0 && G[minVertex][j] < dist[j])
            {
#pragma omp critical
                {
                    dist[j] = G[minVertex][j];
                    parent[j] = minVertex;
                }
            }
        }
    }
}

int main()
{
    G = malloc(V * sizeof(int *));
    vis = malloc(V * sizeof(int));
    dist = malloc(V * sizeof(int));
    parent = malloc(V * sizeof(int));

    FILE *file = fopen("Input.txt", "r");
    for (int i = 0; i < V; i++)
    {
        G[i] = malloc(V * sizeof(int));
        for (int j = 0; j < V; j++)
            fscanf(file, "%d", &G[i][j]);
    }

    for (int i = 0; i < V; i++)
    {
        vis[i] = 0;
        dist[i] = INT_MAX;
        parent[i] = -1;
    }

    dist[0] = 0;

    prims();
}