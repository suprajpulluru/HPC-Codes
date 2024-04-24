#include <stdio.h>
#include <stdlib.h>

#define N 3

void Gaussian(double** A,double* B){
	for(int k=0;k<N;k++){
		for(int j=k+1;j<N;j++)A[k][j]/=A[k][k];
		B[k]/=A[k][k];
		A[k][k]=1;
		for(int i=k+1;i<N;i++){
			for(int j=k+1;j<N;j++)A[i][j]-=(A[i][k]*A[k][j]);
			B[i]-=A[i][k]*B[k];
			A[i][k]=0;
		}
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

	Gaussian(A,B);
	
	//Final Matrix
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++)printf("%lf\t",A[i][j]);
		printf("\n\n");
	}
	
	//Final Array
	for(int i=0;i<N;i++)printf("%d ",B[i]);
}