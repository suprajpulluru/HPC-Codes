#include <stdio.h>
#include <stdlib.h>

#define N 100
#define N_BUCKETS 10

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
		QuickSort(arr, low, pi - 1);
		QuickSort(arr, pi + 1, high);
	}
}

void bucketSort(int *A,int size){
	int **buckets = (int **)malloc(N_BUCKETS * sizeof(int *));
	int *split = malloc((N_BUCKETS - 1) * sizeof(int));
	int *bucket_length = malloc(N_BUCKETS * sizeof(int));

	for (int i=0;i<N_BUCKETS;i++){
		buckets[i] = (int *)malloc(N * sizeof(int));
		bucket_length[i] = 0;
	}

	for(int i=0;i<N_BUCKETS-1;i++)
		split[i] = A[rand() % N];

	QuickSort(split, 0, N_BUCKETS - 2);

	for (int i = 0; i < N; i++)
	{
		int added = 0;
		for (int j = 0; j < N_BUCKETS - 1; j++)
		{
			if (A[i] <= split[j])
			{
				buckets[j][bucket_length[j]++] = A[i];
				added = 1;
				break;
			}
		}
		if (!added){
			buckets[N_BUCKETS - 1][bucket_length[N_BUCKETS - 1]++] = A[i];
        }
	}
	for (int i = 0; i < N_BUCKETS; i++){
		QuickSort(buckets[i], 0, bucket_length[i] - 1);
    }

	int counter = 0;
	for (int i = 0; i < N_BUCKETS; i++){
		for (int j = 0; j < bucket_length[i]; j++){
			A[counter++] = buckets[i][j];
        }
    }
}

int main(){
    FILE *file = fopen("Input.txt", "r");
	int *A = malloc(N * sizeof(int));

	for(int i=0;i<N;i++)fscanf(file,"%d",&A[i]);
	
	fclose(file);

	bucketSort(A, N);

    for(int i=0;i<N;i++)printf("%d\n",A[i]);
}