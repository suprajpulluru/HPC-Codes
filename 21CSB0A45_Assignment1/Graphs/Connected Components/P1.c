#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define V 5 // V -- Number of vertices.

void DFS(int **G, int *visited, int ind)
{
	visited[ind] = 1;
	printf("%d ", ind);
	for (int i = 0; i < V; i++)
	{
		if (G[ind][i] == 1 && visited[i] == 0)
			DFS(G, visited, i);
	}
}

int main()
{
	FILE *fin = fopen("Input.txt", "r");
	if (!fin)
	{
		printf("File cannot be read\n");
		exit(EXIT_FAILURE);
	}

	int **G; // Adjacency Matrix of the Graph.
	G = malloc((V) * sizeof(int *));
	for (int i = 0; i < V; i++)
	{
		G[i] = malloc((V) * sizeof(int));
		for (int j = 0; j < V; j++)
		{
			fscanf(fin, "%d", &G[i][j]);
		}
	}

	int *visited;
	visited = malloc(V * sizeof(int));
	for (int i = 0; i < V; i++)
		visited[i] = 0;

	printf("Connected Components are: \n");
	for (int i = 0; i < V; i++)
	{
		if (visited[i] == 0)
		{
			DFS(G, visited, i);
			printf("\n");
		}
	}
}
