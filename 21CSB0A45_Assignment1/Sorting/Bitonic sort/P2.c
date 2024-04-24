#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1024

int* A;

struct thread_args{
	int s;
	int e;
	int dir;
};

void swap(int a,int b,int order){
	if(order && A[b] < A[a]){
		int temp=A[b];
		A[b]=A[a];
		A[a]=temp;
	}
	if(!order && A[a] < A[b]){
		int temp=A[b];
		A[b]=A[a];
		A[a]=temp;
	}
}

void* bitonicSort(void* arg){
	struct thread_args* T=(struct thread_args*)(arg);
	int s=T->s;
	int e=T->e;
	int dir=T->dir;
	int n=e-s+1;
	if(n==1)return NULL;

	pthread_t tid[2];
	for (int i=s;i-s<n/2;i++)swap(i,n/2+i,dir);

	struct thread_args P[2];
	P[0].s=s;P[0].e=s+n/2-1;P[0].dir=dir;
	pthread_create(&tid[0],NULL,bitonicSort,&P[0]);
	P[1].s=s+n/2;P[1].e=e;P[1].dir=dir;
	pthread_create(&tid[1],NULL,bitonicSort,&P[1]);

	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
}

void bitonicSequenceSort(int s,int e){
	int n=e-s+1;
	for(int j=2;j<=n;j=j*2){
		struct thread_args T[n/j];
		pthread_t tid[n/j];

		for(int i=0;i<n;i=i+j){
			T[i/j].s=i;
			T[i/j].e=i+j-1;

			if(((i/j)%2)==0)T[i/j].dir=1;
			else T[i/j].dir=0;

			pthread_create(&tid[i/j],NULL,bitonicSort,&T[i/j]);
		}

		for (int i=0;i<n;i=i+j)pthread_join(tid[i/j],NULL);
	}
}

int main(){
	A=malloc(N*sizeof(int));

	FILE* fin=fopen("Input.txt","r");
	for(int i=0;i<N;i++)fscanf(fin,"%d",&A[i]);
	
	bitonicSequenceSort(0,N-1);

	for(int i=0;i<N;i++)printf("%d ",A[i]);
}