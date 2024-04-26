#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>

#define V 9

int **G;
int *vis;
int *distance;
int *parent;

void dijkstra()
{
#pragma omp parallel for schedule(static)
    for (int i = 0; i < V; i++)
    {
        int minDist = INT_MAX;
        int minVertex = -1;

#pragma omp parallel for reduction(min : minDist) shared(minVertex)
        for (int j = 0; j < V; j++)
        {
            if (vis[j] == 0 && distance[j] < minDist)
            {
                minDist = distance[j];
                minVertex = j;
            }
        }

        vis[minVertex] = 1;

#pragma omp parallel for shared(minVertex)
        for (int j = 0; j < V; j++)
        {
            if (vis[j] == 0 && G[minVertex][j] && distance[minVertex] != INT_MAX && distance[minVertex] + G[minVertex][j] < distance[j])
            {
#pragma omp critical
                {
                    if (distance[minVertex] + G[minVertex][j] < distance[j])
                    {
                        distance[j] = distance[minVertex] + G[minVertex][j];
                        parent[j] = minVertex;
                    }
                }
            }
        }
    }
}

int main()
{
    G = malloc(V * sizeof(int *));
    vis = malloc(V * sizeof(int));
    distance = malloc(V * sizeof(int));
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
        distance[i] = INT_MAX;
        parent[i] = -1;
    }

    distance[0] = 0;

    dijkstra();

    printf("Distance Table : \n");
    for (int i = 0; i < V; i++)
    {
        if (distance[i] != INT_MAX)
            printf("%d\t", distance[i]);
        else
            printf("Not reachable\t");
    }
}