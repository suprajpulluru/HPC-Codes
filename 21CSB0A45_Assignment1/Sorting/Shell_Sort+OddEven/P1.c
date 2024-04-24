#include <stdio.h>
#include <stdlib.h>

#define SIZE 10000



void OddEvenTransposition(int *A, int size)
{
	for (int i = 1; i <= size / 2; i++)
		if (i % 2 != 0)
		{
			for (int j = 0; j < size / 2; j++)
				if (A[2 * j] > A[2 * j + 1])
				{
					int t = A[2 * j];
					A[2 * j] = A[2 * j + 1];
					A[2 * j + 1] = t;
				}
		}
		else
		{
			for (int j = 1; j < size / 2; j++)
				if (A[2 * j - 1] > A[2 * j])
				{
					int t = A[2 * j - 1];
					A[2 * j - 1] = A[2 * j];
					A[2 * j] = t;
				}
		}
}

void ShellSort(int *A, int startIndex, int lastIndex)
{
	int start = startIndex;
	int last = lastIndex;
	while (startIndex < lastIndex)
	{
		if (A[startIndex] > A[lastIndex])
		{
			int temp = A[lastIndex];
			A[lastIndex] = A[startIndex];
			A[startIndex] = temp;
		}
		startIndex++;
		lastIndex--;
	}

	int n = last - start + 1;
	if (n <= 2)
		return;

	ShellSort(A, start, start + n / 2 - 1);
	ShellSort(A, start + n / 2, last);
}

int main()
{

	int *A = malloc(SIZE * sizeof(int));

	FILE *file = fopen("Input.txt", "r");
	int val;
	for (int i = 0; i < SIZE; i++)
	{
		fscanf(file, "%d", &val);
		A[i] = val;
	}
	fclose(file);

	ShellSort(A, 0, SIZE - 1);
	OddEvenTransposition(A, SIZE);

	for (int i = 0; i < SIZE; i++)
		printf("%d\t", A[i]);


	return 0;
}