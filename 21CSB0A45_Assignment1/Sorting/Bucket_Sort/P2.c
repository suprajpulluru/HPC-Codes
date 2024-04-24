#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 1000000
#define N_BUCKETS 20


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

void bucketSort(int *A, int size)
{
	int **buckets = (int **)malloc(N_BUCKETS * sizeof(int *));
	int *split = malloc((N_BUCKETS - 1) * sizeof(int));
	int *bucket_length = malloc(N_BUCKETS * sizeof(int));

	for (int i = 0; i < N_BUCKETS; i++)
	{
		buckets[i] = (int *)malloc(SIZE * sizeof(int));
		bucket_length[i] = 0;
	}

	pthread_t tid[N_BUCKETS];
	struct s_args init_part[N_BUCKETS];
	int start = 0;

	for (int i = 0; i < N_BUCKETS; i++)
	{
		init_part[i].arr = A;
		init_part[i].low = start;
		if (i != (N_BUCKETS - 1))
			init_part[i].high = start + (size / N_BUCKETS) - 1;
		else
			init_part[i].high = size - 1;
		start = init_part[i].high + 1;
		pthread_create(&tid[i], NULL, QuickSort, (void *)&init_part[i]);
	}

	for (int i = 0; i < N_BUCKETS; i++)
		pthread_join(tid[i], NULL);

	int *sample = (int *)malloc(N_BUCKETS * (N_BUCKETS - 1) * sizeof(int));
	start = 0;
	int space = ((size / N_BUCKETS) / N_BUCKETS);
	for (int i = 0; i < N_BUCKETS; i++){
		for (int j = 0; j < N_BUCKETS - 1; j++){
			sample[start++] = A[init_part[i].low + (j + 1) * space];
		}
	}

	struct s_args *sample_args = (struct s_args *)malloc(sizeof(struct s_args));
	sample_args->arr = split;
	sample_args->low = 0;
	sample_args->high = (N_BUCKETS * (N_BUCKETS - 1)) - 1;
	QuickSort((void *)sample_args);

	start = 0;
	space = (N_BUCKETS - 1) / 2;
	for (int i = 0; i < N_BUCKETS - 1; i++)
	{
		split[i] = sample[start + space];
		start += N_BUCKETS;
	}

	for (int i = 0; i < SIZE; i++)
	{
		int added = 0;
		for (int j = 0; j < N_BUCKETS - 1; j++)
		{
			if (A[i] <= split[j])
			{
				buckets[j][bucket_length[j]++] = A[i];
				added = 1;
				break;
			}
		}
		if (!added){
			buckets[N_BUCKETS - 1][bucket_length[N_BUCKETS - 1]++] = A[i];
		}
	}

	pthread_t fid[N_BUCKETS];
	struct s_args final_part[N_BUCKETS];

	for (int i = 0; i < N_BUCKETS; i++)
	{
		final_part[i].arr = buckets[i];
		final_part[i].low = 0;
		final_part[i].high = bucket_length[i] - 1;
		pthread_create(&fid[i], NULL, (void*)bucketSort, (void *)&final_part[i]);
	}

	for (int i = 0; i < N_BUCKETS; i++){
		pthread_join(fid[i], NULL);
	}

	int counter = 0;
	for (int i = 0; i < N_BUCKETS; i++){
		for (int j = 0; j < bucket_length[i]; j++){
			A[counter++] = buckets[i][j];
		}
	}
}

int main()
{

    FILE *file = fopen("../Input.txt", "r");

	int *A = malloc(SIZE * sizeof(int));

	int val;
	for (int i = 0; i < SIZE; i++)
	{
		fscanf(file, "%d", &val);
		A[i] = val;
	}
	fclose(file);

	bucketSort(A, SIZE);
	
    for (int i = 0; i < SIZE; i++)
		if ((i + 1) % 20 == 0)
			printf("%d\n", A[i]);
		else
			printf("%d\t", A[i]);

	return 0;
}