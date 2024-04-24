#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10000

struct s_args
{
	int low;
	int high;
	int *arr;
};

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

void *QuickSort(void *args)
{
	struct s_args *arg = (struct s_args *)args;
	int *arr = arg->arr;
	int low = arg->low;
	int high = arg->high;

	if (low < high)
	{
		struct s_args part[2];
		pthread_t tid[2];
		int pi = partition(arr, low, high);
		part[0].arr = arr;
		part[1].arr = arr;

		part[0].low = low;
		part[0].high = pi - 1;
		pthread_create(&tid[0], NULL, QuickSort, (void *)&part[0]);

		part[1].low = pi + 1;
		part[1].high = high;
		pthread_create(&tid[1], NULL, QuickSort, (void *)&part[1]);

		pthread_join(tid[0], NULL);
		pthread_join(tid[1], NULL);
	}

	return (NULL);
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

    struct s_args *sample_args = (struct s_args *)malloc(sizeof(struct s_args));
	sample_args->arr = A;
	sample_args->low = 0;
	sample_args->high = SIZE - 1;
	QuickSort((void *)sample_args);


    for (int i = 0; i < SIZE; i++)
			printf("%d\t", A[i]);

	return 0;
}