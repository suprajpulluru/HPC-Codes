#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

#define V 9 

struct ThreadArgs{
    int start;
    int end;
};

int** G;
int** dist;
int num_threads=4; 
pthread_barrier_t barrier; 

void* threadFun(void* arg){
    struct ThreadArgs* args=(struct ThreadArgs*)arg;
    int start=args->start;
    int end=args->end;

    for(int k=0;k<V;k++){
        for(int i=start;i<end;i++){
            for(int j=0;j<V;j++){
                if(dist[i][k]!=INT_MAX && dist[k][j]!=INT_MAX && (dist[i][k]+dist[k][j] < dist[i][j]))dist[i][j]=dist[i][k]+dist[k][j];
            }
        }
        pthread_barrier_wait(&barrier);
    }
}

void FloydsParallel(){
    pthread_t tid[num_threads];
    struct ThreadArgs threadArgs[num_threads];

    dist=malloc(V*sizeof(int*));
    for(int i=0;i<V;i++){
        dist[i]=malloc(V*sizeof(int));
        for(int j=0;j<V;j++){
            if(i==j)dist[i][j] = 0;
            else if(G[i][j]!=0)dist[i][j]=G[i][j];
            else dist[i][j]=INT_MAX;
        }
    }

    int x=(V/num_threads);
    
    pthread_barrier_init(&barrier,NULL,num_threads);
    
    for(int i=0;i<num_threads;i++){
        threadArgs[i].start=i*x;
        if(i==num_threads-1)threadArgs[i].end=V;
        else threadArgs[i].end=(i+1)*x;
        pthread_create(&tid[i],NULL,threadFun,&threadArgs[i]);
    }

    for(int i=0;i<num_threads;i++)pthread_join(tid[i],NULL);

    for(int i=0;i<V;i++){
        printf("Distance from vertex %d to:\n",i);
        for(int j=0;j<V;j++){
            printf("%d: ",j);
            if(dist[i][j] != INT_MAX)printf("%d\n",dist[i][j]);
            else printf("Not reachable\n");
        }
    }
}

int main(){
    FILE* fin=fopen("Input.txt","r");
    if(!fin){
        printf("File cannot be read\n");
        exit(EXIT_FAILURE);
    }
    
	G=malloc(V * sizeof(int*));
    for(int i=0;i<V;i++){
        G[i]=malloc(V * sizeof(int));
        for(int j=0;j<V;j++){
            fscanf(fin,"%d",&G[i][j]);
        }
    }

    FloydsParallel(G);
}