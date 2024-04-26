#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

#define V 9

int **G;
int **D;

void FloydsParallel()
{
    D = malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++)
    {
        D[i] = malloc(V * sizeof(int));
        for (int j = 0; j < V; j++)
        {
            if (i == j)
                D[i][j] = 0;
            else if (G[i][j] != 0)
                D[i][j] = G[i][j];
            else
                D[i][j] = INT_MAX;
        }
    }

#pragma omp parallel shared(D) num_threads(omp_get_max_threads())
    {
        for (int k = 0; k < V; k++)
        {
#pragma omp for collapse(2)
            for (int i = 0; i < V; i++)
            {
                for (int j = 0; j < V; j++)
                {
                    if (D[i][k] != INT_MAX && D[k][j] != INT_MAX && (D[i][k] + D[k][j] < D[i][j]))
                    {
#pragma omp critical
                        {
                            if (D[i][k] + D[k][j] < D[i][j])
                                D[i][j] = D[i][k] + D[k][j];
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < V; i++)
    {
        printf("D from vertex %d to:\n", i);
        for (int j = 0; j < V; j++)
        {
            printf("%d: ", j);
            if (D[i][j] != INT_MAX)
                printf("%d\n", D[i][j]);
            else
                printf("Not reachable\n");
        }
    }
}

int main()
{
    G = malloc(V * sizeof(int *));

    FILE *file = fopen("Input.txt", "r");
    for (int i = 0; i < V; i++)
    {
        G[i] = malloc(V * sizeof(int));
        for (int j = 0; j < V; j++)
            fscanf(file, "%d", &G[i][j]);
    }
    fclose(file);

    FloydsParallel(G);
}