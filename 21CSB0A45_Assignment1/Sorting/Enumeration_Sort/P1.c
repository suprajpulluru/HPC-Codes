#include <stdio.h>
#include <stdlib.h>

#define SIZE 10000


int find_pos(int *A, int size, int idx)
{
	int pos = 0;
	int cmp = A[idx];
	for (int j = 0; j < size; j++)
	{
		if (idx != j)
		{
			if (A[j] < cmp)
				pos++;
			else if (A[j] == cmp && idx > j)
				pos++;
		}
	}
	return pos;
}

void EnumerationSort(int *A, int *B, int size)
{
	for (int i = 0; i < size; i++)
	{
		int pos = find_pos(A, size, i);
		B[pos] = A[i];
	}
}

int main()
{
	FILE *file = fopen("Input.txt", "r");

    int *A = malloc(SIZE * sizeof(int));
	int *B = malloc(SIZE * sizeof(int));

	int val;
	for (int i = 0; i < SIZE; i++)
	{
		fscanf(file, "%d", &val);
		A[i] = val;
		B[i] = 0;
	}
	fclose(file);

	EnumerationSort(A, B, SIZE);
	
    for (int i = 0; i < SIZE; i++)
		if ((i + 1) % 20 == 0)
			printf("%d\n", B[i]);
		else
			printf("%d\t", B[i]);

	return 0;
}