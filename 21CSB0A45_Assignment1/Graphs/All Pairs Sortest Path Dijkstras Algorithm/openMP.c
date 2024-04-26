#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

#define V 9

int **G;
int **distance;

void dijkstras(int source)
{
    distance[source][source] = 0;

    int *vis = malloc(V * sizeof(int));
    for (int i = 0; i < V; i++)
        vis[i] = 0;

    for (int i = 0; i < V; i++)
    {
        int idx = -1;
        for (int j = 0; j < V; j++)
            if (vis[j] == 0 && (idx == -1 || distance[source][j] < distance[source][idx]))
                idx = j;

        vis[idx] = 1;
        if (distance[source][idx] == INT_MAX)
            break;

        for (int j = 0; j < V; j++)
            if (G[idx][j] != 0 && distance[source][idx] + G[idx][j] < distance[source][j])
                distance[source][j] = distance[source][idx] + G[idx][j];
    }
}

void DijkstrasParallel()
{
#pragma omp parallel for schedule(static)
    for (int i = 0; i < V; i++)
        dijkstras(i);

    for (int i = 0; i < V; i++)
    {
        printf("Distance from vertex %d to:\n", i);
        for (int j = 0; j < V; j++)
        {
            printf("%d: ", j);
            if (distance[i][j] != INT_MAX)
                printf("%d\n", distance[i][j]);
            else
                printf("Not reachable\n");
        }
    }
}

int main()
{
    G = malloc(V * sizeof(int *));
    distance = malloc(V * sizeof(int *));

    FILE *file = fopen("./Input.txt", "r");
    for (int i = 0; i < V; i++)
    {
        G[i] = malloc(V * sizeof(int));
        distance[i] = malloc(V * sizeof(int));
        for (int j = 0; j < V; j++)
        {
            fscanf(file, "%d", &G[i][j]);
            distance[i][j] = INT_MAX;
        }
    }
    fclose(file);

    DijkstrasParallel();

    return 0;
}