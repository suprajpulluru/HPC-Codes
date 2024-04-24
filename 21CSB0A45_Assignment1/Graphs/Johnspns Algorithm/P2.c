#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

#define VERT 4
#define INF INT_MAX

struct Edge {
    int src, dest, weight;
};

struct Graph {
    int V;
    int E;
    struct Edge* edge;
};

struct ThreadData {
    struct Graph* graph;
    int src;
    int* dist;
};

struct Graph* createGraph(int V, int E) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = VERT;
    graph->E = E;
    graph->edge = (struct Edge*) malloc(graph->E * sizeof(struct Edge));
    return graph;
}

void printSolution(int dist[][VERT]) {
    printf("The shortest distance matrix:\n");
    for (int i = 0; i < VERT; i++) {
        for (int j = 0; j < VERT; j++) {
            if (dist[i][j] == INF)
                printf("%7s", "INF");
            else
                printf("%7d", dist[i][j]);
        }
        printf("\n");
    }
}

void* BellmanFord(void* data) {
    struct ThreadData* td = (struct ThreadData*) data;
    int V = td->graph->V;
    int E = td->graph->E;

    int src = td->src;
    int* dist = td->dist;

    for (int i = 0; i < V; i++)
        dist[i] = INF;
    dist[src] = 0;

    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = td->graph->edge[j].src;
            int v = td->graph->edge[j].dest;
            int weight = td->graph->edge[j].weight;
            if (dist[u] != INF && dist[u] + weight < dist[v])
                dist[v] = dist[u] + weight;
        }
    }

    pthread_exit(NULL);
}

void Johnson(struct Graph* graph) {
    int V = graph->V;
    int E = graph->E;
    int dist[V];

    struct Graph* tempGraph = createGraph(V + 1, E + V);
    for (int i = 0; i < V; i++) {
        tempGraph->edge[i].src = V;
        tempGraph->edge[i].dest = i;
        tempGraph->edge[i].weight = 0;
    }
    for (int i = 0; i < E; i++) {
        tempGraph->edge[V + i].src = graph->edge[i].src;
        tempGraph->edge[V + i].dest = graph->edge[i].dest;
        tempGraph->edge[V + i].weight = graph->edge[i].weight;
    }

    pthread_t threads[V];
    struct ThreadData td[V];

    for (int i = 0; i < V; i++) {
        td[i].graph = tempGraph;
        td[i].src = V;
        td[i].dist = dist;
        pthread_create(&threads[i], NULL, BellmanFord, (void*) &td[i]);
    }

    for (int i = 0; i < V; i++) {
        pthread_join(threads[i], NULL);
    }

    int h[V];
    for (int i = 0; i < V; i++) {
        h[i] = dist[i];
    }

     for (int i = 0; i < E; i++) {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        graph->edge[i].weight = graph->edge[i].weight + h[u] - h[v];
    }

    int distMatrix[V][V];
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            distMatrix[i][j] = INF;
        }
    }

    for (int i = 0; i < V; i++) {
        
        for (int j = 0; j < V; j++) {
            dist[j] = INF;
        }

       
        dist[i] = 0;
        for (int count = 0; count < V - 1; count++) {
            int u = -1;
            for (int j = 0; j < E; j++) {
                int src = graph->edge[j].src;
                int dest = graph->edge[j].dest;
                int weight = graph->edge[j].weight;
                if (dist[src] != INF && dist[src] + weight < dist[dest]) {
                    dist[dest] = dist[src] + weight;
                    u = dest;
                }
            }
            if (u == -1) break; 
        }
        for (int j = 0; j < V; j++) {
            distMatrix[i][j] = dist[j];
        }
    }

    for (int i = 0; i < E; i++) {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        graph->edge[i].weight = graph->edge[i].weight - h[u] + h[v];
    }
    printSolution(distMatrix);
	return;
}

int main() {
    struct Graph* graph = createGraph(VERT, 5);

    graph->edge[0].src = 0;
    graph->edge[0].dest = 1;
    graph->edge[0].weight = -1;

    graph->edge[1].src = 0;
    graph->edge[1].dest = 2;
    graph->edge[1].weight = 4;

    graph->edge[2].src = 1;
    graph->edge[2].dest = 2;
    graph->edge[2].weight = 3;

    graph->edge[3].src = 1;
    graph->edge[3].dest = 3;
    graph->edge[3].weight = 2;

    graph->edge[4].src = 2;
    graph->edge[4].dest = 3;
    graph->edge[4].weight = 1;

    Johnson(graph);

    return 0;
}
