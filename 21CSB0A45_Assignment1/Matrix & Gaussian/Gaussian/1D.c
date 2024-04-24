#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 3

struct thread_args{
	int p;
	int q;
	double** A;
	double* B;
};

void* compute(void* args){
	struct thread_args* arg=(struct thread_args*)args;
	int q=arg->q;
	int p=arg->p;
	double** A=arg->A;
	double* B=arg->B;

	for(int j=q+1;j<N;j++)A[p][j]-=(A[p][q]*A[q][j]);
	B[p]-=A[p][q]*B[q];
	A[p][q]=0;
}

void Gaussian1D(double** A,double* B){
	pthread_t tid[N];
	struct thread_args args[N];
	for(int k=0;k<N;k++){
		for(int j=k+1;j<N;j++)A[k][j]/=A[k][k];
		B[k]/=A[k][k];
		A[k][k]=1;
		for(int i=k+1;i<N;i++){
			args[i].A=A;
			args[i].B=B;
			args[i].p=i;
			args[i].q=k;
			pthread_create(&tid[i],NULL,compute,&args[i]);
		}
		for(int i=k+1;i<N;i++)pthread_join(tid[i], NULL);
	}
}

int main(){
	double** A=(double**)malloc(N*sizeof(double*));
	double* B=malloc(N*sizeof(double*));

	for(int i=0;i<N;i++){
		A[i]=(double*)malloc(N*sizeof(double));
		B[i]=0;
	}

	FILE* fin=fopen("Input.txt","r");
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++)fscanf(fin,"%lf",&A[i][j]);
	}

	for(int i=0;i<N;i++)fscanf(fin,"%lf",&B[i]);


	Gaussian1D(A, B);

	//Final Matrix
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++)printf("%lf\t",A[i][j]);
		printf("\n\n");
	}
	
	//Final Array
	for(int i=0;i<N;i++)printf("%d ",B[i]);
}