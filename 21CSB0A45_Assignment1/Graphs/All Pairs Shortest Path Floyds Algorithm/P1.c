#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define V 9 //V -- Number of vertices.

void Floyds(int** G){
	int** dist;
	dist= malloc((V)*sizeof(int*));
	for(int i=0;i<V;i++){
		dist[i] = malloc((V)*sizeof(int));
		for(int j=0;j<V;j++){
			if(i==j)dist[i][j]=0;
			else if(G[i][j]!=0)dist[i][j]=G[i][j];
			else dist[i][j]=INT_MAX;
		}
	}
	
	for(int i=0;i<V;i++){//Treating i as intermediate node.
		for(int j=0;j<V;j++){
			for(int k=0;k<V;k++){
				if(dist[j][i]!=INT_MAX && dist[i][k]!=INT_MAX && dist[j][i]+dist[i][k] < dist[j][k]){
					dist[j][k]=dist[j][i]+dist[i][k];
				}
			}
		}
	}
	
	for(int i=0;i<V;i++){
		printf("Distance from vertex %d to:\n",i);
		for(int j=0;j<V;j++){
			printf("%d: ",j);
			if(dist[i][j]!=INT_MAX)printf("%d\n",dist[i][j]);
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
	
	int** G;//Adjacency Matrix of the Graph.
	G = malloc((V)*sizeof(int*));
   for(int i=0;i<V;i++){
   	G[i] = malloc((V)*sizeof(int));
   	for(int j=0;j<V;j++){
   		fscanf(fin,"%d",&G[i][j]);
   	}
   }	
   
   Floyds(G);
}
