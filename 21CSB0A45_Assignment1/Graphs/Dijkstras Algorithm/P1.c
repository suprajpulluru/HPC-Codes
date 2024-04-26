#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define V 9

void dijkstras(int **G, int *D, int src)
{
	D[src] = 0;

	int *visited = malloc(V * sizeof(int));
	for (int i = 0; i < V; i++)
		visited[i] = 0;

	for (int i = 0; i < V; i++)
	{
		int ind = -1;
		for (int j = 0; j < V; j++)
		{
			if (visited[j] == 0 && (ind == -1 || D[j] < D[ind]))
				ind = j;
		}
		visited[ind] = 1;
		if (D[ind] == INT_MAX)
			break;
		for (int j = 0; j < V; j++)
		{
			if (G[ind][j] != 0 && D[ind] + G[ind][j] < D[j])
				D[j] = D[ind] + G[ind][j];
		}
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

	int **G = malloc(V * sizeof(int *));
	for (int i = 0; i < V; i++)
	{
		G[i] = malloc(V * sizeof(int));
		for (int j = 0; j < V; j++)
			fscanf(fin, "%d", &G[i][j]);
	}

	int *D = malloc(V * sizeof(int));
	for (int i = 0; i < V; i++)
		D[i] = INT_MAX;

	dijkstras(G, D, 0);

	printf("Distance from vertex %d to:\n", 0);
	for (int i = 0; i < V; i++)
	{
		printf("%d: ", i);
		if (D[i] != INT_MAX)
			printf("%d\n", D[i]);
		else
			printf("Not reachable\n");
	}
}