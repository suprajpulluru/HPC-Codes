//Source Partitioned Formulation -- According to Textbook.

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

#define V 9
#define NTHREADS 3

int** G;
int** result;

void dijkstras(int src){
	result[src][src]=0;
	
	int* visited=malloc(V*sizeof(int));//visited[i] -- indicates whether i is visited/not during the algorithm.
	for(int i=0;i<V;i++)visited[i]=0;
	
	for(int i=0;i<V;i++){
		int ind=-1;
		for(int j=0;j<V;j++){
			if(visited[j]==0 && (ind==-1 || result[src][j] < result[src][ind]))ind=j;
		}	
		visited[ind]=1;
		if(result[src][ind]==INT_MAX)break;
		for(int j=0;j<V;j++){
			if(G[ind][j]!=0 && result[src][ind]+G[ind][j] < result[src][j])result[src][j]=result[src][ind]+G[ind][j];
		}
	}
}

void* threadFun(void* arg){
	int start=*((int*)arg);
	for(int i=start;i<start+(V/NTHREADS);i++){
		dijkstras(i);
	}
}

int main(){
	FILE* fin=fopen("Input.txt","r");	
	if(!fin){
		printf("File cannot be read\n");
		exit(EXIT_FAILURE);
	}
	
	G = malloc((V)*sizeof(int*));
	result = malloc(V*sizeof(int*));
   for(int i=0;i<V;i++){
   	G[i] = malloc((V)*sizeof(int));
   	result[i] = malloc((V)*sizeof(int));
   	for(int j=0;j<V;j++){
   		fscanf(fin,"%d",&G[i][j]);
   		result[i][j]=INT_MAX;
   	}
   }	   
   
   pthread_t* tid=malloc(NTHREADS * sizeof(pthread_t));
   int* thread_args=malloc(NTHREADS * sizeof(int));
   for(int i=0;i<NTHREADS;i++){
	   thread_args[i]=i*(V/NTHREADS);
	   pthread_create(&tid[i],NULL,threadFun,&thread_args[i]);
   }
   
   for(int i=0;i<NTHREADS;i++){
   	pthread_join(tid[i],NULL);
   }
   
   for(int i=0;i<V;i++){
		printf("Distance from vertex %d to:\n",i);
		for(int j=0;j<V;j++){
			printf("%d: ",j);
			if(result[i][j]!=INT_MAX)printf("%d\n",result[i][j]);
			else printf("Not reachable\n");
		}
	}
}
