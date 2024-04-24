#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10000

struct thread_args
{
	int i;
	int *arr;
	int size;
	int res;
};


void *find_pos(void *args)
{
	struct thread_args *arg = (struct thread_args *)args;
	int *A = arg->arr;
	int i = arg->i;
	int size = arg->size;

	int cmp = A[i];
	int pos = 0;
	for (int j = 0; j < size; j++)
	{
		if (i != j)
		{
			if (A[j] < cmp)
				pos++;
			else if (A[j] == cmp && i > j)
				pos++;
		}
	}
	arg->res = pos;
	pthread_exit(NULL);
}

void EnumerationSort(int *A, int *B, int size)
{
	pthread_t tid[size];
	struct thread_args args[size];

	for (int i = 0; i < size; i++)
	{
		args[i].arr = A;
		args[i].i = i;
		args[i].size = size;

		pthread_create(&tid[i], NULL, find_pos, (void *)&args[i]);
	}

	for (int i = 0; i < size; i++)
	{
		pthread_join(tid[i], NULL);
		B[args[i].res] = A[i];
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
		if ((i + 1) % 10 == 0)
			printf("%d\n", B[i]);
		else
			printf("%d\t", B[i]);

	return 0;

}