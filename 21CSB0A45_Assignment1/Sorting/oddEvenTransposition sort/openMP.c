#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10

int* A;

void swap(int* X,int* Y){
    int temp=*X;
    *X=*Y;
    *Y=temp;
}

void oddEvenTranspositionSort(){
    #pragma omp parallel num_threads(N)
	{
        int id=omp_get_thread_num();
        int i=0;
        while(i<N){
            if(i%2 == 0 && id%2 == 0 && id < N-1 && A[id] > A[id+1])swap(&A[id],&A[id+1]);
            else if(i%2 == 1 && id%2 == 1 && id < N-1 && A[id] > A[id+1])swap(&A[id],&A[id+1]);
            #pragma omp barrier
            i++;
        }
    }
}

int main() {
    A=malloc(N * sizeof(int));

    FILE* fin=fopen("Input.txt","r");
    for(int i=0;i<N;i++)fscanf(fin,"%d",&A[i]);

    oddEvenTranspositionSort();

    for(int i=0;i<N;i++) printf("%d ",A[i]);

    free(A);
}