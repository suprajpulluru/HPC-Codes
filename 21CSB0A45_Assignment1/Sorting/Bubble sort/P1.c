#include <stdio.h>
#include <stdlib.h>

#define N 10

void bubbleSort(int* A){
	for(int i=N-1;i>=0;i--){
		for(int j=0;j<i;j++){
			if(A[j] > A[j+1]){
				int temp=A[j];
				A[j]=A[j+1];
				A[j+1]=temp;
			}	
		}
	}
}

int main(){
	FILE* fin=fopen("Input.txt","r");
	if(!fin){
		printf("Cannot read the input file\n");
		exit(EXIT_FAILURE);
	}
	
	int* A=malloc(N*sizeof(int));
	for(int i=0;i<N;i++)fscanf(fin,"%d",&A[i]);
	
	bubbleSort(A);
	
	for(int i=0;i<N;i++)printf("%d ",A[i]);
	printf("\n");
}