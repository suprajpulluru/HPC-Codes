#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 100
#define N_BUCKETS 10

struct s_args
{
    int low;
    int high;
    int *arr;
};

int partition(int *arr, int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return (i + 1);
}

void QuickSort(int *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

#pragma omp parallel sections
        {
#pragma omp section
            QuickSort(arr, low, pi - 1);
#pragma omp section
            QuickSort(arr, pi + 1, high);
        }
    }
}

void bucketSort(int *A, int size)
{
    int **buckets = (int **)malloc(N_BUCKETS * sizeof(int *));
    int *split = malloc((N_BUCKETS - 1) * sizeof(int));
    int *bucket_length = malloc(N_BUCKETS * sizeof(int));

    for (int i = 0; i < N_BUCKETS; i++)
    {
        buckets[i] = (int *)malloc(SIZE * sizeof(int));
        bucket_length[i] = 0;
    }

    int *sample = (int *)malloc(N_BUCKETS * (N_BUCKETS - 1) * sizeof(int));
    int start = 0;
    int space = ((size / N_BUCKETS) / N_BUCKETS);
    for (int i = 0; i < N_BUCKETS; i++)
    {
        for (int j = 0; j < N_BUCKETS - 1; j++)
        {
            sample[start++] = A[i * (size / N_BUCKETS) + (j + 1) * space];
        }
    }

    QuickSort(sample, 0, N_BUCKETS * (N_BUCKETS - 1) - 1);

    start = 0;
    space = (N_BUCKETS - 1) / 2;
    for (int i = 0; i < N_BUCKETS - 1; i++)
    {
        split[i] = sample[start + space];
        start += N_BUCKETS;
    }

    for (int i = 0; i < SIZE; i++)
    {
        int added = 0;
        for (int j = 0; j < N_BUCKETS - 1; j++)
        {
            if (A[i] <= split[j])
            {
#pragma omp critical
                {
                    buckets[j][bucket_length[j]++] = A[i];
                }
                added = 1;
                break;
            }
        }
        if (!added)
        {
#pragma omp critical
            {
                buckets[N_BUCKETS - 1][bucket_length[N_BUCKETS - 1]++] = A[i];
            }
        }
    }

#pragma omp parallel for
    for (int i = 0; i < N_BUCKETS; i++)
    {
        QuickSort(buckets[i], 0, bucket_length[i] - 1);
    }

    int counter = 0;
    for (int i = 0; i < N_BUCKETS; i++)
    {
        for (int j = 0; j < bucket_length[i]; j++)
        {
            A[counter++] = buckets[i][j];
        }
    }

    for (int i = 0; i < N_BUCKETS; i++)
    {
        free(buckets[i]);
    }
    free(buckets);
    free(split);
    free(bucket_length);
    free(sample);
}

int main()
{
    FILE *file = fopen("../Input.txt", "r");
    int *A = malloc(SIZE * sizeof(int));

    int val;
    for (int i = 0; i < SIZE; i++)
    {
        fscanf(file, "%d", &val);
        A[i] = val;
    }
    fclose(file);

    bucketSort(A, SIZE);

    for (int i = 0; i < SIZE; i++)
    {
        if ((i + 1) % 20 == 0)
            printf("%d\n", A[i]);
        else
            printf("%d ", A[i]);
    }

    free(A);

    return 0;
}