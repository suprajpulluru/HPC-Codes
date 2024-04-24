#include <stdio.h>
#include <stdlib.h>

#define N 10

void oddEvenTransposition(int* A){
	for(int i=0;i<N;i++){
		if((i%2) == 0){
			for(int j=0;j+1<N;j+=2){
				if(A[j] > A[j+1]){
					int temp=A[j];
					A[j]=A[j+1];
					A[j+1]=temp;
				}
			}
		}
		else{
			for(int j=1;j+1<N;j+=2){
				if(A[j] > A[j+1]){
					int temp=A[j];
					A[j]=A[j+1];
					A[j+1]=temp;
				}
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
	
	oddEvenTransposition(A);

	for(int i=0;i<N;i++)printf("%d ",A[i]);
	printf("\n");
}