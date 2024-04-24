#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

#define V 9
#define NTHREADS 3

int** G;
int* parent;
int* visited;
int* dist;
int minVertex;
int minDistance;
pthread_mutex_t mutex;

void* findMin(void* arg){
	int start=*((int*)arg);
	for(int i=start;(i<V && i<start+(V/NTHREADS));i++){
		if(visited[i]==1)continue;
		pthread_mutex_lock(&mutex);
		if(dist[i] < minDistance){
			minVertex=i;
			minDistance=dist[i];
		}
		pthread_mutex_unlock(&mutex);
	}
}

void* updateDist(void* arg){
	int start=*((int*)arg);
	for(int i=start;(i<V && i<start+(V/NTHREADS));i++){
		if(visited[i]==1)continue;
		if(G[minVertex][i]!=0 && G[minVertex][i] < dist[i]){
			dist[i]=G[minVertex][i];
			parent[i]=minVertex;
		}
	}
}

int main(){
	FILE* fin=fopen("Input.txt","r");	
	if(!fin){
		printf("File cannot be read\n");
		exit(EXIT_FAILURE);
	}
	
	G = malloc((V)*sizeof(int*));
   for(int i=0;i<V;i++){
   	G[i] = malloc((V)*sizeof(int));
   	for(int j=0;j<V;j++){
   		fscanf(fin,"%d",&G[i][j]);
   	}
   }	
   
   parent=malloc(V*sizeof(int));
   visited=malloc(V*sizeof(int));
   dist=malloc(V*sizeof(int));
   
   for(int i=0;i<V;i++){
   	parent[i]=-1;
   	visited[i]=0;
   	dist[i]=INT_MAX;
   }
   
   dist[0]=0;
   
   pthread_mutex_init(&mutex,NULL);
   
   printf("Edge \tWeight\n");
	for(int i=0;i<V;i++){
		minVertex=-1;
		minDistance=INT_MAX;
		
		pthread_t* tid=malloc(NTHREADS * sizeof(pthread_t));
		int* thread_args=malloc(V * sizeof(int));
    
		for(int j=0;j<NTHREADS;j++){
			thread_args[j]=j*(V/NTHREADS);
			pthread_create(&tid[j],NULL,findMin,&thread_args[j]);
		}
    
    	for(int j=0;j<NTHREADS;j++){
			pthread_join(tid[j],NULL);
		}	
		
		if(minVertex!=0)printf("%d - %d \t%d \n",parent[minVertex],minVertex,G[minVertex][parent[minVertex]]);
		visited[minVertex]=1;
	
		for(int j=0;j<NTHREADS;j++){
			thread_args[j]=j*(V/NTHREADS);
			pthread_create(&tid[j],NULL,updateDist,&thread_args[j]);
		}
    
    	for(int j=0;j<NTHREADS;j++){
			pthread_join(tid[j],NULL);
		}	
   }
}
