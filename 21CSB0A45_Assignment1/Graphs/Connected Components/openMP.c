#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define V 5

int **G;

int *parent;
int *rank;
omp_lock_t *mutex;

int find(int i)
{
    if (parent[i] != i)
        parent[i] = find(parent[i]);
    return parent[i];
}

void unionSet(int u, int v)
{
    int pu = find(u);
    int pv = find(v);

    if (pu != pv)
    {
        if (rank[pu] > rank[pv])
        {
            omp_set_lock(&mutex[pv]);
            parent[pv] = pu;
            omp_unset_lock(&mutex[pv]);
        }
        else if (rank[pu] < rank[pv])
        {
            omp_set_lock(&mutex[pu]);
            parent[pu] = pv;
            omp_unset_lock(&mutex[pu]);
        }
        else
        {
            omp_set_lock(&mutex[pv]);
            parent[pv] = pu;
            omp_unset_lock(&mutex[pv]);
            omp_set_lock(&mutex[pu]);
            rank[pu]++;
            omp_unset_lock(&mutex[pu]);
        }
    }
}

void makeSet(int i)
{
    parent[i] = i;
    rank[i] = 0;
}

void DFS(int i)
{
#pragma omp parallel for
    for (int j = 0; j < V; j++)
    {
        if (G[i][j] == 1)
        {
            int u = i;
            int v = j;
            if (find(u) != find(v))
            {
                unionSet(u, v);
            }
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

    parent = malloc(V * sizeof(int));
    rank = malloc(V * sizeof(int));
    mutex = malloc(V * sizeof(omp_lock_t));

    for (int i = 0; i < V; i++)
    {
        parent[i] = i;
        rank[i] = 0;
        omp_init_lock(&mutex[i]);
    }

#pragma omp parallel for
    for (int i = 0; i < V; i++)
    {
        DFS(i);
    }

    printf("Connected Components:\n");
#pragma omp parallel for
    for (int i = 0; i < V; i++)
    {
        if (parent[i] == i)
        {
#pragma omp critical
            {
                for (int j = 0; j < V; j++)
                {
                    if (i == find(j))
                        printf("%d ", j);
                }
                printf("\n");
            }
        }
    }

    return 0;
}