#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000

struct thread_args{
	int i;
	int* A;
	int res;
};

void *find_pos(void *args){
	struct thread_args *arg = (struct thread_args *)args;
	int* A=arg->A;
	int i=arg->i;

	int pos=0;
	for(int j=0;j<N;j++){
		if(A[j] < A[i])pos++;
		else if (A[j] == A[i] && i > j)pos++;
	}
	arg->res=pos;
}

void EnumerationSort(int* A,int* B){
	struct thread_args args[N];

	#pragma omp parallel num_threads(N)
    {
        int i=omp_get_thread_num();
		args[i].A=A;
		args[i].i=i;
		find_pos((void*)&args[i]);
        B[args[i].res]=A[i];
	}
}

int main(){
	FILE *file=fopen("Input.txt", "r");

    int* A = malloc(N * sizeof(int));
	int* B = malloc(N * sizeof(int));

	for(int i=0;i<N;i++){
		fscanf(file,"%d",&A[i]);
		B[i]=0;
	}
	fclose(file);

    EnumerationSort(A,B);
	
    for(int i=0;i<N;i++)printf("%d\n",B[i]);
}