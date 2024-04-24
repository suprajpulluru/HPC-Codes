#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 1000

pthread_mutex_t mutex;

struct thread_args
{
	int *out;
	int *count;
	int *arr;
	int exp;
	int i;
};


int get_max(int *arr, int n)
{
	int mx = arr[0];
	for (int i = 1; i < n; i++)
		if (arr[i] > mx)
			mx = arr[i];
	return mx;
}

void *position(void *args)
{
	struct thread_args *arg = (struct thread_args *)args;
	int *out = arg->out;
	int *count = arg->count;
	int *arr = arg->arr;
	int exp = arg->exp;
	int i = arg->i;

	pthread_mutex_lock(&mutex);
	out[count[(arr[i] / exp) % 10] - 1] = arr[i];
	count[(arr[i] / exp) % 10]--;
	pthread_mutex_unlock(&mutex);

	return NULL;
}

void count_sort(int *arr, int n, int exp)
{
	int out[n];
	pthread_t tid[n];
	struct thread_args arg[n];
	int i, count[10] = {0};

	for (i = 0; i < n; i++)
		count[(arr[i] / exp) % 10]++;

	for (i = 1; i < 10; i++)
		count[i] += count[i - 1];

	for (i = n - 1; i >= 0; i--)
	{
		arg[i].out = out;
		arg[i].count = count;
		arg[i].arr = arr;
		arg[i].exp = exp;
		arg[i].i = i;
		pthread_create(&tid[i], NULL, position, (void *)&arg[i]);
	}

	for (i = n - 1; i >= 0; i--)
		pthread_join(tid[i], NULL);

	for (i = 0; i < n; i++)
		arr[i] = out[i];
}

void RadixSort(int *arr, int n)
{
	int m = get_max(arr, n);
	pthread_mutex_init(&mutex, NULL);

	for (int exp = 1; m / exp > 0; exp *= 10)
		count_sort(arr, n, exp);
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

	RadixSort(A, SIZE);
	
    for (int i = 0; i < SIZE; i++)
			printf("%d\t", A[i]);

	return 0;
}