#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <omp.h>

#define N 16

struct Thread_args
{
    int (*A)[N];
    int (*B)[N];
    int (*R)[N];
};
void *multiply(void *args)
{
    struct Thread_args *arg = (struct Thread_args *)(args);
    int(*A)[N] = arg->A;
    int(*B)[N] = arg->B;
    int(*R)[N] = arg->R;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            R[i][j] = A[i][j] * B[i][j];
}

int main()
{
    int **A = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
        A[i] = (int *)malloc(N * sizeof(int));

    int **B = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
        B[i] = (int *)malloc(N * sizeof(int));

    int **C = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
        C[i] = (int *)malloc(N * sizeof(int));

    FILE *file = fopen("Input.txt", "r");
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            fscanf(file, "%d", &A[j][i]);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            fscanf(file, "%d", &B[j][i]);
            C[i][j] = 0;
        }

    int ABrod[N][N][N], BBrod[N][N][N], Res[N][N][N];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
            {
                ABrod[i][k][j] = A[i][j];
                BBrod[i][j][k] = B[j][i];
            }

    struct Thread_args arg[N];

#pragma omp parallel num_threads(N)
    {
        int i = omp_get_thread_num();
        arg[i].A = ABrod[i];
        arg[i].B = BBrod[i];
        arg[i].R = Res[i];
        multiply((void *)&arg[i]);
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C[i][j] += Res[k][i][j];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%d\t", C[i][j]);
        printf("\n");
    }
    printf("\n");
    return 0;
}