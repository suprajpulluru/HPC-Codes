#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define V 5

int** G; 

int* parent;
int* rank;
pthread_mutex_t* mutex;

void makeSet(int i){
    parent[i]=i;
    rank[i]=0;
}

int find(int i){
    if(parent[i]!=i)parent[i]=find(parent[i]);
    return parent[i];
}

void unionSets(int u,int v){
	int pu=find(u);
	int pv=find(v);

	if(pu!=pv){
		if(rank[pu] < rank[pv]){
			pthread_mutex_lock(&mutex[pu]);
			parent[pu]=pv;
			pthread_mutex_unlock(&mutex[pu]);
		}
		else if(rank[pu] > rank[pv]){
			pthread_mutex_lock(&mutex[pv]);
			parent[pv]=pu;
			pthread_mutex_unlock(&mutex[pv]);
		}
      else{
      	pthread_mutex_lock(&mutex[pv]);
			parent[pv]=pu;
			pthread_mutex_unlock(&mutex[pv]);
			pthread_mutex_lock(&mutex[pu]);
			rank[pu]++;
			pthread_mutex_unlock(&mutex[pu]);
		}
	}
}

void* DFS(void* arg){
	int i=*((int*)arg);

	for(int j=0;j<V;j++){
		if(G[i][j]==1){
			if(find(i)!=find(j))unionSets(i,j);
		}
	}
	pthread_exit(NULL);
}

int main(){
    FILE* fin=fopen("Input.txt","r");
    if(!fin){
        printf("File cannot be read\n");
        exit(EXIT_FAILURE);
    }

    G=malloc(V*sizeof(int*));
    for(int i=0;i<V;i++){
        G[i]=malloc(V*sizeof(int));
        for(int j=0;j<V;j++){
            fscanf(fin,"%d",&G[i][j]);
        }
    }
    
    parent=malloc(V*sizeof(int));
    rank=malloc(V*sizeof(int));
    mutex=malloc(V*sizeof(pthread_mutex_t));
    
	for(int i=0;i<V;i++){
		parent[i]=i;
		rank[i]=0;
		pthread_mutex_init(&mutex[i],NULL);
	}

    pthread_t* tid=malloc(V*sizeof(pthread_t));
    int* thread_args=malloc(V*sizeof(int));
    
    for(int i=0;i<V;i++){
    	  thread_args[i]=i;
        pthread_create(&tid[i],NULL,DFS,&thread_args[i]);
    }
    
    for(int i=0;i<V;i++){
        pthread_join(tid[i],NULL);
    }
	 	
    printf("Connected Components:\n");
    for (int i=0;i<V;i++){
        if(parent[i]==i){
            for(int j=0;j<V;j++){
                if(find(j)==i)printf("%d ",j);
            }
            printf("\n");
        }
    }
}
