#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 3
#define NUM_PER_LINE 3

struct thread_args
{
    int i;
    int k;
    double **A;
    double *B;
};

struct compute_args
{
    int i;
    int j;
    int k;
    double **A;
};

void print_array(double *A, int size)
{

    for (int i = 0; i < size; i++)
        if ((i + 1) % NUM_PER_LINE == 0)
            printf("%lf\n", A[i]);
        else
            printf("%lf\t", A[i]);

    return;
}

void print_matrix(double **mat)
{

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
            printf("%lf\t", mat[i][j]);
        printf("\n\n");
    }
    printf("\n\n");
    return;
}

void compute(void *args)
{
    struct compute_args *arg = (struct compute_args *)args;
    int i = arg->i;
    int j = arg->j;
    int k = arg->k;
    double **A = arg->A;

    A[i][j] -= (A[i][k] * A[k][j]);
    return;
}

void iterate(void *args)
{
    struct thread_args *arg = (struct thread_args *)args;
    struct compute_args data[SIZE];

    int k = arg->k;
    int i = arg->i;
    double **A = arg->A;
    double *B = arg->B;
    for (int j = k + 1; j < SIZE; j++)
    {
        data[j].A = A;
        data[j].i = i;
        data[j].j = j;
        data[j].k = k;

        compute((void *)&data[j]);
    }

    B[i] -= A[i][k] * B[k];
    A[i][k] = 0;

    return;
}

void Gaussian(double **A, double *B)
{
    struct thread_args args[SIZE];
    for (int k = 0; k < SIZE; k++)
    {
        for (int j = k + 1; j < SIZE; j++)
            A[k][j] /= A[k][k];
        B[k] /= A[k][k];
        A[k][k] = 1;

        for (int i = k + 1; i < SIZE; i++)
        {
            args[i].A = A;
            args[i].B = B;
            args[i].i = i;
            args[i].k = k;
            iterate((void *)&args[i]);
        }
    }
}

int main()
{
    double **A = (double **)malloc(SIZE * sizeof(double *));
    double *B = malloc(SIZE * sizeof(double *));

    for (int i = 0; i < SIZE; i++)
    {
        A[i] = (double *)malloc(SIZE * sizeof(double));
        B[i] = 0;
    }

    FILE *file = fopen("../Input.txt", "r");
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            fscanf(file, "%lf", &A[i][j]);

    for (int i = 0; i < SIZE; i++)
        fscanf(file, "%lf", &B[i]);

    fclose(file);

    Gaussian(A, B);
    print_matrix(A);
    print_array(B, SIZE);

    return 0;
}