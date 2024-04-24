#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 3

struct compute1_args{
	int i;
	int k;
	double** A;
	double* B;
};

struct compute2_args{
	int i;
	int j;
	int k;
	double** A;
};

void* compute2(void* args){
	struct compute2_args* arg=(struct compute2_args*)args;
	int i=arg->i;
	int j=arg->j;
	int k=arg->k;
	double** A=arg->A;

	A[i][j]-=(A[i][k]*A[k][j]);
}

void* compute1(void* args){
	struct compute1_args* arg=(struct compute1_args*)args;
	pthread_t cid[N];
	struct compute2_args data[N];

	int k=arg->k;
	int i=arg->i;
	double** A=arg->A;
	double* B=arg->B;

	for(int j=k+1;j<N;j++){
		data[j].A=A;
		data[j].i=i;
		data[j].j=j;
		data[j].k=k;

		pthread_create(&cid[j],NULL,compute2,(void *)&data[j]);
	}
	for(int j=k+1;j<N;j++)pthread_join(cid[i],NULL);

	B[i]-=A[i][k]*B[k];
	A[i][k]=0;
}

void Gaussian2D(double** A,double* B){
	pthread_t tid[N];
	struct compute1_args args[N];
	for(int k=0;k<N;k++){
		for(int j=k+1;j<N;j++)A[k][j]/=A[k][k];
		B[k]/=A[k][k];
		A[k][k]=1;
		for (int i=k+1;i<N;i++){
			args[i].A=A;
			args[i].B=B;
			args[i].i=i;
			args[i].k=k;
			pthread_create(&tid[i],NULL,compute1,&args[i]);
		}

		for(int i=k+1;i<N;i++)pthread_join(tid[i],NULL);
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


	Gaussian2D(A, B);

	//Final Matrix
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++)printf("%lf\t",A[i][j]);
		printf("\n\n");
	}
	
	//Final Array
	for(int i=0;i<N;i++)printf("%d ",B[i]);
}