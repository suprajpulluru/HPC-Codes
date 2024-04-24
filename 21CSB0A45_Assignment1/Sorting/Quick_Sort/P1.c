#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000000

int partition(int *arr, int low, int high)
{
	int pivot = arr[high];
	int i = (low - 1);
	for (int j = low; j <= high; j++)
	{
		if (arr[j] < pivot)
		{
			i++;
			int temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}
	int temp = arr[i + 1];
	arr[i + 1] = arr[high];
	arr[high] = temp;

	return (i + 1);
}

void QuickSort(int *arr, int low, int high)
{
	if (low < high)
	{
		int pi = partition(arr, low, high);
		QuickSort(arr, low, pi - 1);
		QuickSort(arr, pi + 1, high);
	}
}

int main()
{

	FILE *file = fopen("Input.txt", "r");

    int *A = malloc(SIZE * sizeof(int));

	int val;
	for (int i = 0; i < SIZE; i++)
	{
		fscanf(file, "%d", &val);
		A[i] = val;
	}
	fclose(file);

	QuickSort(A, 0, SIZE - 1);
	

    for (int i = 0; i < SIZE; i++)
			printf("%d\t", A[i]);

	return 0;
}