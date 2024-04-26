#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define V 9

void LubyAlgorithm(int **G)
{
    int unvisited = V;
    int *visited = (int *)malloc(V * sizeof(int));
    for (int i = 0; i < V; i++)
        visited[i] = 0;

    srand(time(NULL));

    int *R = (int *)malloc(V * sizeof(int));

    printf("Maximal Independent Set:\n");

#pragma omp parallel for shared(visited, unvisited, G, R) schedule(static)
    for (int i = 0; i < V; i++)
    {
        if (visited[i] == 1)
            continue;

        int *curr_visited = (int *)calloc(V, sizeof(int));

        for (int j = 0; j < V; j++)
            R[j] = rand() % V;

        int is_independent = 1;
        for (int j = 0; j < V; j++)
        {
            if (visited[j] == 0 && G[i][j] != 0 && R[i] >= R[j])
            {
                is_independent = 0;
                break;
            }
        }

        if (is_independent)
        {
#pragma omp critical
            {
                visited[i] = 1;
                unvisited--;
                printf("%d ", i);
                for (int j = 0; j < V; j++)
                {
                    if (G[i][j] != 0 && visited[j] == 0 && curr_visited[j] == 0)
                    {
                        curr_visited[j] = 1;
                        unvisited--;
                    }
                }
            }
        }
        free(curr_visited);
    }
    printf("\n");
    free(visited);
    free(R);
}

int main()
{
    int **G;
    G = (int **)malloc(V * sizeof(int *));

    FILE *file = fopen("Input.txt", "r");
    for (int i = 0; i < V; i++)
    {
        G[i] = (int *)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++)
            fscanf(file, "%d", &G[i][j]);
    }
    fclose(file);

    LubyAlgorithm(G);

    for (int i = 0; i < V; i++)
        free(G[i]);
    free(G);

    return 0;
}