#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define V 9 

int* I;
int* C;
int* R;
int** G;
int num_threads=3;
pthread_mutex_t mutex; 
pthread_barrier_t barrier; 

struct ThreadArgs{
    int start;
    int end;
};

int isIndependent(int ind){
    for(int j=0;j<V;j++){
        if(G[ind][j]!=0 && R[ind] > R[j])return 0;
    }
    return 1;
}

void* threadFun(void* arg){
    struct ThreadArgs* args=(struct ThreadArgs*)arg;
    int start=args->start;
    int end=args->end;

    while(1){
        for(int i=start;i<=end;i++){
			if(C[i]==1)R[i]=INT_MAX;
			else R[i]=(rand() % V);
		}

        pthread_barrier_wait(&barrier);
        
        for(int i=start;i<=end;i++){
            if(C[i]==0 && isIndependent(i)){
                pthread_mutex_lock(&mutex);
				I[i]=1;
                C[i]=1;
                for(int j=0;j<V;j++){
                    if(G[i][j]!=0)C[j]=1;
                }
                pthread_mutex_unlock(&mutex);
            }
        }
        pthread_barrier_wait(&barrier);

        int allVisited=1;
        for(int i=start;i<=end;i++){
            if(C[i]==0)allVisited=0;
        }
        if(allVisited)break;
    }
}

void lubyParallel(){
    I=malloc(V * sizeof(int));
    C=malloc(V * sizeof(int));
    for(int i=0;i<V;i++){
        I[i]=0;
		C[i]=0;
    }
    R=malloc(V * sizeof(int));
    
    pthread_mutex_init(&mutex,NULL);
    pthread_barrier_init(&barrier,NULL,num_threads);

	int x=V/num_threads;

    pthread_t tid[num_threads];
    struct ThreadArgs threadArgs[num_threads];
    for(int i=0;i<num_threads;i++){
        threadArgs[i].start=i*x;
        if(i==num_threads-1)threadArgs[i].end=V-1;
        else threadArgs[i].end=(i+1)*x-1;
        pthread_create(&tid[i],NULL,threadFun,&threadArgs[i]);
    }

    for(int i=0;i<V;i++)pthread_join(tid[i], NULL);

    printf("Maximal Independent Set:\n");
    for(int i=0;i<V;i++){
        if(I[i]==1)printf("%d ", i);
    }
    printf("\n");
}

int main(){
	FILE* fin=fopen("Input.txt","r");	
	if(!fin){
		printf("File cannot be read\n");
		exit(EXIT_FAILURE);
	}

    srand(time(NULL));

	G=malloc(V * sizeof(int*));
	for(int i=0;i<V;i++){
   		G[i]=malloc(V*sizeof(int));
	   	for(int j=0;j<V;j++)fscanf(fin,"%d",&G[i][j]);
   	}	

    lubyParallel();
}