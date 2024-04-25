#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1024

struct thread_args
{
	int *A;
	int start;
	int last;
	int dir;
};

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

void *Bitonic_sort(void *arg)
{
	struct thread_args *args = (struct thread_args *)(arg);
	int *A = args->A;
	int start = args->start;
	int last = args->last;
	int dir = args->dir;
	int n = last - start + 1;
	if (n == 1)
		return NULL;

	pthread_t tid[2];
	for (int i = start; i < start + n / 2; i++)
		swap(A, i, n / 2 + i, dir);

	struct thread_args part[2];
	part[0].A = A;
	part[0].start = start;
	part[0].last = start + n / 2 - 1;
	part[0].dir = dir;
	pthread_create(&tid[0], NULL, Bitonic_sort, (void *)&part[0]);
	part[1].A = A;
	part[1].start = start + n / 2;
	part[1].last = last;
	part[1].dir = dir;
	pthread_create(&tid[1], NULL, Bitonic_sort, (void *)&part[1]);

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
}

void BitnoicSequenceGenerator(int *A, int start, int last)
{
	int n = last - start + 1;
	for (int j = 2; j <= n; j = j * 2)
	{
		struct thread_args args[n / j];
		pthread_t tid[n / j];

		for (int i = 0; i < n; i = i + j)
		{
			args[i / j].A = A;
			args[i / j].start = i;
			args[i / j].last = i + j - 1;

			if (((i / j) % 2) == 0)
				args[i / j].dir = 1;
			else
				args[i / j].dir = 0;

			pthread_create(&tid[i / j], NULL, Bitonic_sort, (void *)&args[i / j]);
		}

		for (int i = 0; i < n; i = i + j)
			pthread_join(tid[i / j], NULL);
	}
}

int main()
{
	int *A = malloc(N * sizeof(int));

	FILE *fin = fopen("Input.txt", "r");
	for (int i = 0; i < N; i++)
		fscanf(fin, "%d", &A[i]);

	fclose(fin);

	BitnoicSequenceGenerator(A, 0, N - 1);

	for (int i = 0; i < N; i++)
		printf("%d\n", A[i]);
}