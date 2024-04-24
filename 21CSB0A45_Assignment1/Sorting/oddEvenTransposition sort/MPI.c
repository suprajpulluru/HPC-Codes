#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 10

int* A;

void swap(int* X,int* Y) {
    int temp=*X;
    *X=*Y;
    *Y=temp;
}

void oddEvenTranspositionSort(int id,int size){
    int i = 0;
    while (i < N){
        if (i%2 == 0 && id%2 == 0 && id < size-1 && A[id] > A[id+1])swap(&A[id],&A[id+1]);
        else if (i%2 == 1 && id%2 == 1 && id < size-1 && A[id] > A[id+1])swap(&A[id],&A[id+1]);
        MPI_Barrier(MPI_COMM_WORLD);
        i++;
    }
}

int main(int argc, char *argv[]) {
    int id, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (id == 0) {
        A = malloc(N * sizeof(int));
        FILE* fin = fopen("Input.txt", "r");
        for (int i = 0; i < N; i++) fscanf(fin, "%d", &A[i]);
        fclose(fin);
    }

    MPI_Bcast(A, N, MPI_INT, 0, MPI_COMM_WORLD);
    oddEvenTranspositionSort(id, size);

    if (id == 0) {
        for (int i = 0; i < N; i++) printf("%d ", A[i]);
        printf("\n");
    }

    free(A);
    MPI_Finalize();
    return 0;
}