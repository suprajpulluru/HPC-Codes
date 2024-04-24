#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10000

struct Thread_args
{
	int si;
	int li;
	int *arr;
};

void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}



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

void *ShellSort(void *args)
{
	struct Thread_args *arg = (struct Thread_args *)args;
	int *A = arg->arr;
	int si = arg->si;
	int li = arg->li;

	int start = si;
	int last = li;
	while (si < li)
	{
		if (A[si] > A[li])
			swap(&A[si], &A[li]);
		si++;
		li--;
	}

	int n = last - start + 1;
	if (n <= 2)
		return NULL;

	pthread_t tid[2];
	struct Thread_args part[2];
	part[0].arr = A;
	part[0].si = start;
	part[0].li = start + n / 2 - 1;
	pthread_create(&tid[0], NULL, ShellSort, (void *)&part[0]);
	part[1].arr = A;
	part[1].si = start + n / 2;
	part[1].li = last;
	pthread_create(&tid[1], NULL, ShellSort, (void *)&part[1]);

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	return NULL;
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

	struct Thread_args arg;
	arg.arr = A;
	arg.si = 0;
	arg.li = SIZE - 1;

	ShellSort((void *)&arg);
	OddEvenTransposition(A, SIZE);

	for (int i = 0; i < SIZE; i++)
		printf("%d\t", A[i]);

	return 0;
}