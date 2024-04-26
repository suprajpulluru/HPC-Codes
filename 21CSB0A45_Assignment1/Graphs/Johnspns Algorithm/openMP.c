#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>

#define VERTICES 4
#define INFINITY INT_MAX
#define NUM_THREADS 2

struct Edge
{
    int source;
    int destination;
    int weight;
};

struct Graph
{
    int vertices;
    int edges;
    struct Edge *edge;
};

struct ThreadData
{
    struct Graph *graph;
    int start;
    int end;
    int *distances;
};

struct Graph *createGraph(int vertices, int edges)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->vertices = vertices; // Fix: Assign vertices passed as argument
    graph->edges = edges;
    graph->edge = (struct Edge *)malloc(graph->edges * sizeof(struct Edge));
    return graph;
}

void partialBellmanFord(void *arg)
{
    struct ThreadData *data = (struct ThreadData *)arg;
    struct Graph *graph = data->graph;
    int start = data->start;
    int end = data->end;
    int *distances = data->distances;

    int V = graph->vertices;
    int E = graph->edges;

    for (int i = start; i < end; i++)
        distances[i] = INFINITY;
    distances[start] = 0;

    for (int i = 1; i <= V - 1; i++)
        for (int j = 0; j < E; j++)
        {
            int source = graph->edge[j].source;
            int destination = graph->edge[j].destination;
            int weight = graph->edge[j].weight;
            if (distances[source] != INFINITY && distances[source] + weight < distances[destination])
                distances[destination] = distances[source] + weight;
        }
}

void parallelBellmanFord(struct Graph *graph, int src, int distances[])
{
    struct ThreadData threadData[NUM_THREADS];

    int chunk = graph->vertices / NUM_THREADS; // Fix: Calculate chunk size based on the number of vertices

#pragma omp parallel for
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threadData[i].graph = graph;
        threadData[i].start = i * chunk;
        threadData[i].end = (i + 1) * chunk;
        threadData[i].distances = distances;
        partialBellmanFord((void *)&threadData[i]);
    }
}

void johnsonsAlgorithm(struct Graph *graph)
{
    int V = graph->vertices;
    int E = graph->edges;
    int distances[V];

    struct Graph *tempGraph = createGraph(V + 1, E + V);
    for (int i = 0; i < V; i++)
    {
        tempGraph->edge[i].source = V;
        tempGraph->edge[i].destination = i;
        tempGraph->edge[i].weight = 0;
    }
    for (int i = 0; i < E; i++)
    {
        tempGraph->edge[V + i].source = graph->edge[i].source;
        tempGraph->edge[V + i].destination = graph->edge[i].destination;
        tempGraph->edge[V + i].weight = graph->edge[i].weight;
    }

    parallelBellmanFord(tempGraph, V, distances);

    int d[V];
    for (int i = 0; i < V; i++)
        d[i] = distances[i];

    for (int i = 0; i < E; i++)
    {
        int source = graph->edge[i].source;
        int destination = graph->edge[i].destination;
        graph->edge[i].weight = graph->edge[i].weight + d[source] - d[destination];
    }

    int distanceMatrix[V][V];
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            distanceMatrix[i][j] = INFINITY;

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
            distances[j] = INFINITY;

        distances[i] = 0;
        for (int count = 0; count < V - 1; count++)
        {
            int u = -1;
            for (int j = 0; j < E; j++)
            {
                int source = graph->edge[j].source;
                int destination = graph->edge[j].destination;
                int weight = graph->edge[j].weight;
                if (distances[source] != INFINITY && distances[source] + weight < distances[destination])
                {
                    distances[destination] = distances[source] + weight;
                    u = destination;
                }
            }
            if (u == -1)
                break;
        }

        for (int j = 0; j < V; j++)
            distanceMatrix[i][j] = distances[j];
    }

    for (int i = 0; i < E; i++)
    {
        int source = graph->edge[i].source;
        int destination = graph->edge[i].destination;
        graph->edge[i].weight = graph->edge[i].weight - d[source] + d[destination];
    }

    for (int i = 0; i < VERTICES; i++)
    {
        for (int j = 0; j < VERTICES; j++)
        {
            if (distanceMatrix[i][j] == INFINITY)
                printf("%7s", "INF");
            else
                printf("%7d", distanceMatrix[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    struct Graph *graph = createGraph(VERTICES, 5);

    FILE *file = fopen("Input.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        if (fscanf(file, "%d %d %d", &(graph->edge[i].source), &(graph->edge[i].destination), &(graph->edge[i].weight)) != 3)
        {
            printf("Error reading from file.\n");
            fclose(file);
            return 1;
        }
    }
    fclose(file);

    johnsonsAlgorithm(graph);

    return 0;
}