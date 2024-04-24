#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define V 9 //V -- Number of vertices.

void prims(int** G){
	int* parent=malloc(V*sizeof(int));
	int* visited=malloc(V*sizeof(int));
	int* dist=malloc(V*sizeof(int));
	
	parent[0]=-1;
	dist[0]=0;
	for(int i=1;i<V;i++){
		visited[i]=0;
		dist[i]=INT_MAX;
	}
	
	for(int i=0;i<V;i++){
		int ind=-1;
		for(int j=0;j<V;j++){
			if(visited[j]==0 && (ind==-1 || dist[j] < dist[ind]))ind=j;
		}
		visited[ind]=1;
		for(int j=0;j<V;j++){
			if(visited[j]==0 && G[ind][j]!=0 && G[ind][j] < dist[j]){
				parent[j]=ind;
				dist[j]=G[ind][j];
			}
		}
	}

	printf("Edge \tWeight\n");
	for(int i=1;i<V;i++){
		printf("%d - %d \t%d \n",parent[i],i,G[i][parent[i]]);
	}
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
   
   prims(G);
}
