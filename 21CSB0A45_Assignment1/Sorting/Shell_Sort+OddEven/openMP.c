#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

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
                    swap(&A[2 * j], &A[2 * j + 1]);
                }
        }
        else
        {
            for (int j = 1; j < size / 2; j++)
                if (A[2 * j - 1] > A[2 * j])
                {
                    swap(&A[2 * j - 1], &A[2 * j]);
                }
        }
}

void ShellSort(int *A, int si, int li)
{
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
        return;

#pragma omp parallel sections
    {
#pragma omp section
        {
            ShellSort(A, start, start + n / 2 - 1);
        }
#pragma omp section
        {
            ShellSort(A, start + n / 2, last);
        }
    }
}

int main()
{
    int *A = malloc(SIZE * sizeof(int));

    FILE *file = fopen("../Input.txt", "r");
    int val;
    for (int i = 0; i < SIZE; i++)
    {
        fscanf(file, "%d", &val);
        A[i] = val;
    }
    fclose(file);

#pragma omp parallel
    {
#pragma omp single nowait
        {
            ShellSort(A, 0, SIZE - 1);
        }
    }

    OddEvenTransposition(A, SIZE);

    for (int i = 0; i < SIZE; i++)
        printf("%d\t", A[i]);

    free(A);

    return 0;
}