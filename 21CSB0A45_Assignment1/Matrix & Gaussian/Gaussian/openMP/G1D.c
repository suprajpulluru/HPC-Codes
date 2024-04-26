#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MATRIX_SIZE 3
#define NUM_PER_LINE 3

struct ThreadArgs
{
    int row;
    int col;
    double **matrix;
    double *vector;
};

void rowOperation(void *arguments)
{
    struct ThreadArgs *args = (struct ThreadArgs *)arguments;
    int row = args->row;
    int col = args->col;
    double **matrix = args->matrix;
    double *vector = args->vector;

    for (int j = col + 1; j < MATRIX_SIZE; j++)
        matrix[row][j] -= (matrix[row][col] * matrix[col][j]);
    vector[row] -= matrix[row][col] * vector[col];
    matrix[row][col] = 0;
}

void gaussianElimination(double **matrix, double *vector)
{
    struct ThreadArgs threadArgs[MATRIX_SIZE];
    for (int col = 0; col < MATRIX_SIZE; col++)
    {
        for (int j = col + 1; j < MATRIX_SIZE; j++)
            matrix[col][j] /= matrix[col][col];
        vector[col] /= matrix[col][col];
        matrix[col][col] = 1;

#pragma omp parallel for
        for (int row = col + 1; row < MATRIX_SIZE; row++)
        {
            threadArgs[row].matrix = matrix;
            threadArgs[row].vector = vector;
            threadArgs[row].row = row;
            threadArgs[row].col = col;
            rowOperation((void *)&threadArgs[row]);
        }
    }
}

int main()
{
    double **matrix = (double **)malloc(MATRIX_SIZE * sizeof(double *));
    double *vector = malloc(MATRIX_SIZE * sizeof(double *));

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        matrix[i] = (double *)malloc(MATRIX_SIZE * sizeof(double));
        vector[i] = 0;
    }

    FILE *file = fopen("Input.txt", "r");
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            fscanf(file, "%lf", &matrix[i][j]);

    for (int i = 0; i < MATRIX_SIZE; i++)
        fscanf(file, "%lf", &vector[i]);

    fclose(file);

    gaussianElimination(matrix, vector);

    printf("Resultant Matrix: \n");

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
            printf("%lf\t", matrix[i][j]);
        printf("\n\n");
    }

    printf("Resultant vector : \n");

    for (int i = 0; i < MATRIX_SIZE; i++)
        if ((i + 1) % NUM_PER_LINE == 0)
            printf("%lf\n", vector[i]);
        else
            printf("%lf\t", vector[i]);

    return 0;
}