#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

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

void Bitonic_sort(struct thread_args *args)
{
    int *A = args->A;
    int start = args->start;
    int last = args->last;
    int dir = args->dir;
    int n = last - start + 1;
    if (n == 1)
        return;

#pragma omp parallel sections
    {
#pragma omp section
        {
            for (int i = start; i - start < n / 2; i++)
                swap(A, i, n / 2 + i, dir);
        }
#pragma omp section
        {
            struct thread_args part[2];
            part[0].A = A;
            part[0].start = start;
            part[0].last = start + n / 2 - 1;
            part[0].dir = dir;
            Bitonic_sort(&part[0]);
            part[1].A = A;
            part[1].start = start + n / 2;
            part[1].last = last;
            part[1].dir = dir;
            Bitonic_sort(&part[1]);
        }
    }
}

void BitnoicSequenceGenerator(int *A, int start, int last)
{
    int n = last - start + 1;
    for (int j = 2; j <= n; j = j * 2)
    {
        struct thread_args args[n / j];
#pragma omp parallel for
        for (int i = 0; i < n; i = i + j)
        {
            args[i / j].A = A;
            args[i / j].start = i;
            args[i / j].last = i + j - 1;

            if (((i / j) % 2) == 0)
                args[i / j].dir = 1;
            else
                args[i / j].dir = 0;

            Bitonic_sort(&args[i / j]);
        }
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