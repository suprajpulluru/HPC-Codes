#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10000

struct s_args
{
    int low;
    int high;
    int *arr;
};

int partition(int *arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++)
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

void *QuickSort(void *args)
{
    struct s_args *arg = (struct s_args *)args;
    int *arr = arg->arr;
    int low = arg->low;
    int high = arg->high;

    if (low < high)
    {
        int pi = partition(arr, low, high);

        struct s_args part1 = {low, pi - 1, arr};
        struct s_args part2 = {pi + 1, high, arr};

        pthread_t tid[2];
        pthread_create(&tid[0], NULL, QuickSort, (void *)&part1);
        pthread_create(&tid[1], NULL, QuickSort, (void *)&part2);

        pthread_join(tid[0], NULL);
        pthread_join(tid[1], NULL);
    }

    return NULL;
}

int main()
{
    FILE *file = fopen("Input.txt", "r");
    if (!file)
    {
        printf("Error: File cannot be opened\n");
        return -1;
    }

    int *A = malloc(SIZE * sizeof(int));
    if (!A)
    {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return -1;
    }

    int val;
    for (int i = 0; i < SIZE; i++)
    {
        if (fscanf(file, "%d", &val) != 1)
        {
            printf("Error: Unable to read input\n");
            free(A);
            fclose(file);
            return -1;
        }
        A[i] = val;
    }
    fclose(file);

    struct s_args sample_args = {0, SIZE - 1, A};
    QuickSort((void *)&sample_args);

    for (int i = 0; i < SIZE; i++)
        printf("%d\t", A[i]);

    free(A);

    return 0;
}