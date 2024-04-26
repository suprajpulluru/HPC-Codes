#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 3

struct Thread_args
{
    int i;
    int k;
    double **A;
    double *B;
};

void *iterate(void *args)
{
    struct Thread_args *arg = (struct Thread_args *)args;
    int k = arg->k;
    int i = arg->i;
    double **A = arg->A;
    double *B = arg->B;

    for (int j = k + 1; j < SIZE; j++)
        A[i][j] -= (A[i][k] * A[k][j]);
    B[i] -= A[i][k] * B[k];
    A[i][k] = 0;

    pthread_exit(NULL);
}

void Gaussian(double **A, double *B)
{
    pthread_t tid[SIZE];
    struct Thread_args args[SIZE];
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
            pthread_create(&tid[i], NULL, iterate, (void *)&args[i]);
        }

        for (int i = k + 1; i < SIZE; i++)
            pthread_join(tid[i], NULL);
    }
}

void backSubstitution(int N, double **X, double *y, double *Z)
{
    int i, j;
    for (i = N - 1; i >= 0; i--)
    {
        int count = 0;
        Z[i] = y[i];
        for (j = i + 1; j < N; j++)
        {
            Z[i] -= X[i][j] * Z[j];
        }
        Z[i] = Z[i] / X[i][i];
    }
}

int main()
{
    double **A = (double **)malloc(SIZE * sizeof(double *));
    double *B = malloc(SIZE * sizeof(double *));
    double *sol = malloc(SIZE * sizeof(double *));

    for (int i = 0; i < SIZE; i++)
    {
        A[i] = (double *)malloc(SIZE * sizeof(double));
        B[i] = 0;
    }

    FILE *input_file = fopen("Input.txt", "r");
    char line[10];

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            fgets(line, sizeof(line), input_file);
            A[i][j] = atoi(line);
        }
    }

    for (int i = 0; i < SIZE; i++)
    {
        fgets(line, sizeof(line), input_file);
        B[i] = atoi(line);
    }

    fclose(input_file);

    Gaussian(A, B);

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
            printf("%lf\t", A[i][j]);
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++)
    {
        printf("%lf\t", B[i]);
    }

    backSubstitution(SIZE, A, B, sol);

    printf("\n\n Solution array after backsubsitution:\n \n");

    for (int i = 0; i < SIZE; i++)
    {
        printf("%lf\t", sol[i]);
    }

    printf("\n");

    return 0;
}