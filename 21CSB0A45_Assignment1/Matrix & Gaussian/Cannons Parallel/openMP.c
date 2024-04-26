#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define N 1000
#define NUM_THREADS 10

int **A;
int **B;
int **C;

void shift_left(int **mat, int i, int n, int amount)
{
    int *temp_buffer = (int *)malloc(sizeof(int) * n);
    for (int j = 0; j < n; j++)
        temp_buffer[j] = mat[i][(j + amount) % n];

    for (int j = 0; j < n; j++)
        mat[i][j] = temp_buffer[j];

    free(temp_buffer);
}

void shift_up(int **mat, int j, int n, int amount)
{
    int *temp_buffer = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
        temp_buffer[i] = mat[(i + amount) % n][j];

    for (int i = 0; i < n; i++)
        mat[i][j] = temp_buffer[i];

    free(temp_buffer);
}

void compute_product(int start_row, int end_row)
{
    int i, j, k;

    for (i = start_row; i < end_row; i++)
    {
        shift_left(A, i, N, i);
    }

#pragma omp parallel for private(j, k) shared(A, B, C)
    for (j = 0; j < N; j++)
    {
        shift_up(B, j, N, j);

        for (i = start_row; i < end_row; i++)
        {
            for (k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void cannon_mul(int **mat1, int **mat2, int **rez, int n)
{
    int chunk_size = N / NUM_THREADS;
    int remainder = N % NUM_THREADS;
    int start_row = 0;
    int end_row = 0;

#pragma omp parallel num_threads(NUM_THREADS)
    {
        int tid = omp_get_thread_num();
        start_row = tid * chunk_size;
        end_row = start_row + chunk_size + (tid < remainder ? 1 : 0);

        compute_product(start_row, end_row);
    }
}

int main()
{
    FILE *fin = fopen("Input.txt", "r");
    FILE *fout = fopen("output1.txt", "w");
    if (!fin || !fout)
    {
        printf("File cannot be read or written\n");
        exit(EXIT_FAILURE);
    }

    A = (int **)(malloc((N) * sizeof(int *)));
    for (int i = 0; i < N; i++)
    {
        A[i] = (int *)(malloc((N) * sizeof(int)));
        for (int j = 0; j < N; j++)
        {
            fscanf(fin, "%d", &A[i][j]);
        }
    }

    B = (int **)(malloc((N) * sizeof(int *)));
    for (int i = 0; i < N; i++)
    {
        B[i] = (int *)(malloc((N) * sizeof(int)));
        for (int j = 0; j < N; j++)
        {
            fscanf(fin, "%d", &B[i][j]);
        }
    }

    C = (int **)(malloc((N) * sizeof(int *)));
    for (int i = 0; i < N; i++)
    {
        C[i] = (int *)(malloc((N) * sizeof(int)));
        for (int j = 0; j < N; j++)
        {
            C[i][j] = 0;
        }
    }

    cannon_mul(A, B, C, N);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            fprintf(fout, "%d ", C[i][j]);
        }
        fprintf(fout, "\n");
    }

    fclose(fin);
    fclose(fout);

    return 0;
}