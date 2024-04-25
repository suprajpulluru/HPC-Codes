#include <stdio.h>
#include <stdlib.h>

#define N 10000

int find_pos(int* A,int idx){
	int pos=0;
	int cmp=A[idx];
	for(int j=0;j<N;j++){
		if(idx!=j){
			if(A[j] < cmp)pos++;
			else if (A[j] == cmp && j < idx)pos++;
		}
	}
	return pos;
}

void EnumerationSort(int* A,int* B){
	for(int i=0;i<N;i++){
		int pos=find_pos(A,i);
		B[pos]=A[i];
	}
}

int main(){
	FILE* file=fopen("Input.txt","r");

    int* A = malloc(N * sizeof(int));
	int* B = malloc(N * sizeof(int));

	for(int i=0;i<N;i++){
		fscanf(file,"%d",&A[i]);
		B[i]=0;
	}
	fclose(file);

	EnumerationSort(A,B);
	
    for(int i=0;i<N;i++)printf("%d\n",B[i]);
}