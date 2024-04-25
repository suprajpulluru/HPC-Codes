#include <stdio.h>
#include <stdlib.h>

#define N 1024

void swap(int *A, int l, int r, int order)
{
	if (order == 1 && A[l] > A[r])
	{
		int temp = A[l];
		A[l] = A[r];
		A[r] = temp;
	}
	else if (order == 0 && A[l] < A[r])
	{
		int temp = A[l];
		A[l] = A[r];
		A[r] = temp;
	}
}

void BitonicSort(int *A, int l, int r, int order)
{
	int n = r - l + 1;
	if (n == 1)
		return;
	for (int i = l; i < l + n / 2; i++)
		swap(A, i, i + n / 2, order);
	BitonicSort(A, l, l + n / 2 - 1, order);
	BitonicSort(A, l + n / 2, r, order);
}

void BitonicSequenceGenerator(int *A, int l, int r)
{
	int n = r - l + 1;
	for (int j = 2; j <= n; j = j * 2)
	{
		for (int i = 0; i < n; i = i + j)
		{
			if ((i / j) % 2 == 0)
				BitonicSort(A, i, i + j - 1, 1);
			else
				BitonicSort(A, i, i + j - 1, 0);
		}
	}
}

int main()
{
	FILE *fin = fopen("Input.txt", "r");
	if (!fin)
	{
		printf("File cannot be open\n");
		exit(EXIT_FAILURE);
	}

	int *A = malloc(N * sizeof(int));
	for (int i = 0; i < N; i++)
		fscanf(fin, "%d", &A[i]);

	BitonicSequenceGenerator(A, 0, N - 1);

	for (int i = 0; i < N; i++)
		printf("%d\n", A[i]);
}