#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define V 9 // V -- Number of vertices.

void Floyds(int **G)
{
	int **D;
	D = malloc((V) * sizeof(int *));
	for (int i = 0; i < V; i++)
	{
		D[i] = malloc((V) * sizeof(int));
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

	for (int i = 0; i < V; i++)
	{ // Treating i as intermediate node.
		for (int j = 0; j < V; j++)
		{
			for (int k = 0; k < V; k++)
			{
				if (D[j][i] != INT_MAX && D[i][k] != INT_MAX && D[j][i] + D[i][k] < D[j][k])
				{
					D[j][k] = D[j][i] + D[i][k];
				}
			}
		}
	}

	for (int i = 0; i < V; i++)
	{
		printf("Dance from vertex %d to:\n", i);
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

	Floyds(G);
}