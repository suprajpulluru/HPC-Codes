#include <stdio.h>
#include <stdlib.h>

#define N 100

int get_max(int *A)
{
	int mx = A[0];
	for (int i = 1; i < N; i++)
		if (A[i] > mx)
			mx = A[i];
	return mx;
}

void enumeration_sort(int *A, int exp, int r)
{
	int output[N];
	int count_size = 1 << r;
	int count[count_size];

	for (int i = 0; i < count_size; i++)
		count[i] = 0;

	for (int i = 0; i < N; i++)
		count[(A[i] >> exp) & (count_size - 1)]++;

	for (int i = 1; i < count_size; i++)
		count[i] += count[i - 1];

	for (int i = N - 1; i >= 0; i--)
	{
		output[count[(A[i] >> exp) & (count_size - 1)] - 1] = A[i];
		count[(A[i] >> exp) & (count_size - 1)]--;
	}

	for (int i = 0; i < N; i++)
		A[i] = output[i];
}

void RadixSort(int *A, int r)
{
	int m = get_max(A);

	int num_bits = 0;
	while ((m >> num_bits) > 0)
		num_bits += r;

	for (int exp = 0; exp < num_bits; exp += r)
		enumeration_sort(A, exp, r);
}

int main()
{
	int *A = malloc(N * sizeof(int));

	FILE *fin = fopen("Input.txt", "r");
	if (!fin)
	{
		printf("Error opening file.\n");
		return 1;
	}
	for (int i = 0; i < N; i++)
		fscanf(fin, "%d", &A[i]);
	fclose(fin);

	RadixSort(A, 2);

	for (int i = 0; i < N; i++)
		printf("%d\n", A[i]);
}