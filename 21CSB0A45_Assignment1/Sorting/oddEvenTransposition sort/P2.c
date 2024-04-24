#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10

int* A;
pthread_barrier_t barrier;

void *swap(int* X,int* Y){
	int temp=*X;
	*X=*Y;
	*Y=temp;
}

void* fun(void* args){
    int id=*(int*)args;
    int i=0;
    while(i < N){
        if(i%2 == 0 && id%2 == 0 && id < N-1 && A[id] > A[id+1])swap(&A[id],&A[id+1]);
        else if(i%2 == 1 && id%2 == 1 && id < N-1 && A[id] > A[id+1])swap(&A[id],&A[id+1]);
        pthread_barrier_wait(&barrier);
        i++;
    }
    return NULL;
}


void oddEvenTranspositionSort(){
	pthread_barrier_init(&barrier,NULL,N);
	pthread_t tid[N];
	int thread_args[N];
	for(int i=0;i<N;i++){
		thread_args[i]=i;
		pthread_create(&tid[i],NULL,fun,&thread_args[i]);
	}
	for(int i=0;i<N;i++)pthread_join(tid[i],NULL);
}

int main(){
	A=malloc(N*sizeof(int));

	FILE* fin=fopen("Input.txt","r");
	for(int i=0;i<N;i++)fscanf(fin,"%d",&A[i]);
	
	oddEvenTranspositionSort();
	
	for(int i=0;i<N;i++)printf("%d ",A[i]);		
}