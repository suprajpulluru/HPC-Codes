#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define V 9 //V -- Number of vertices.

void Luby(int** G){
	int* visited=malloc(V*sizeof(int));
	int* currItrVisited=malloc(V*sizeof(int));
	for(int i=0;i<V;i++)visited[i]=0;
	int unVisited=V;
	
	srand(time(NULL));
	
	int* R=malloc(V*sizeof(int));
	
	printf("Maximal Independent Set:\n");
	while(unVisited>0){
		for(int i=0;i<V;i++){
			R[i]=(rand() % V);
			currItrVisited[i]=0;
		}
		for(int i=0;i<V;i++){
			if(visited[i]==1 || currItrVisited[i]==1)continue;
			int indpt=1;
			for(int j=0;j<V;j++){
				if(visited[j]==0 && G[i][j]!=0 && R[i] >= R[j])indpt=0;
			}
			if(indpt==1){
				printf("%d ",i);
				currItrVisited[i]=1;
				unVisited--;
				for(int j=0;j<V;j++){
					if(G[i][j]!=0 && visited[j]==0 && currItrVisited[j]==0){
						currItrVisited[j]=1;
						unVisited--;
					}
				}
			}
		}
		for(int i=0;i<V;i++){
			if(currItrVisited[i]==1)visited[i]=1;
		}
	}	
	printf("\n");
}

int main(){
	FILE* fin=fopen("Input.txt","r");	
	if(!fin){
		printf("File cannot be read\n");
		exit(EXIT_FAILURE);
	}
	
	int** G;//Adjacency Matrix of the Graph.
	G = malloc((V)*sizeof(int*));
   for(int i=0;i<V;i++){
   	G[i] = malloc((V)*sizeof(int));
   	for(int j=0;j<V;j++){
   		fscanf(fin,"%d",&G[i][j]);
   	}
   }	
   
  	Luby(G);
}
