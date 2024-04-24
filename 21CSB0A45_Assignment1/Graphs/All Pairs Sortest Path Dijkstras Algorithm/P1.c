#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define V 9 //V -- Number of vertices.

void dijkstras(int** G,int src){
	int* dist=malloc(V*sizeof(int));//dist[i] -- distance from src to vertex i.
	for(int i=0;i<V;i++)dist[i]=INT_MAX;
	dist[src]=0;
	
	int* visited=malloc(V*sizeof(int));//visited[i] -- indicates whether i is visited/not during the algorithm.
	for(int i=0;i<V;i++)visited[i]=0;
	
	for(int i=0;i<V;i++){
		int ind=-1;
		for(int j=0;j<V;j++){
			if(visited[j]==0 && (ind==-1 || dist[j] < dist[ind]))ind=j;
		}	
		visited[ind]=1;
		if(dist[ind]==INT_MAX)break;
		for(int j=0;j<V;j++){
			if(G[ind][j]!=0 && dist[ind]+G[ind][j] < dist[j])dist[j]=dist[ind]+G[ind][j];
		}
	}
	
	printf("Distance from vertex %d to:\n",src);
	for(int i=0;i<V;i++){
		printf("%d: ",i);
		if(dist[i]!=INT_MAX)printf("%d\n",dist[i]);
		else printf("Not reachable\n");
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
   
   for(int i=0;i<V;i++){
	   dijkstras(G,i);
   }
}
