#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000000


int get_max(int *arr, int n)
{
	int mx = arr[0];
	for (int i = 1; i < n; i++){
		if (arr[i] > mx){
			mx = arr[i];
        }
    }
	return mx;
}

void count_sort(int *arr, int n, int exp)
{
	int output[n];
	int i, count[10] = {0};

	for (i = 0; i < n; i++){
		count[(arr[i] / exp) % 10]++;
    }

	for (i = 1; i < 10; i++){
		count[i] += count[i - 1];
    }

	for (i = n - 1; i >= 0; i--)
	{
		output[count[(arr[i] / exp) % 10] - 1] = arr[i];
		count[(arr[i] / exp) % 10]--;
	}

	for (i = 0; i < n; i++){
		arr[i] = output[i];
    }
}

void RadixSort(int *arr, int n)
{
	int m = get_max(arr, n);

	for (int exp = 1; m / exp > 0; exp *= 10)
		count_sort(arr, n, exp);
}

int main()
{
	int *A = malloc(SIZE * sizeof(int));

	FILE *file = fopen("Input.txt", "r");
	int val;
	for (int i = 0; i < SIZE; i++)
	{
		fscanf(file, "%d", &val);
		A[i] = val;
	}
	fclose(file);

	RadixSort(A, SIZE);
	
    for (int i = 0; i < SIZE; i++){
		printf("%d\t", A[i]);
    }


	return 0;
}