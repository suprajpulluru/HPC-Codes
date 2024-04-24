#include <stdio.h>
#include <stdlib.h>

#define N 1024

int* A;

void swap(int a,int b,int order){
	if(order && A[b] < A[a]){
		int temp=A[b];
		A[b]=A[a];
		A[a]=temp;
	}
	if(!order && A[a] < A[b]){
		int temp=A[b];
		A[b]=A[a];
		A[a]=temp;
	}
}

void bitonicSort(int s,int e,int dir){
	int n=e-s+1;
	if(n==1)return;
	for(int i=s;(i-s)<n/2;i++)swap(i,n/2+i,dir);
	bitonicSort(s,s+n/2-1,dir);
	bitonicSort(s+n/2,e,dir);
}

void bitonicSequenceSort(int s,int e){
	int n=e-s+1;
	for(int j=2;j<=n;j*=2){
		for(int i=0;i<n;i+=j){
			if(((i/j)%2) == 0)bitonicSort(i,i+j-1,1);
			else bitonicSort(i,i+j-1,0);
		}
	}
}

int main(){
	A=malloc(N*sizeof(int));

	FILE* fin=fopen("Input.txt","r");
	for(int i=0;i<N;i++)fscanf(fin,"%d",&A[i]);
	
	bitonicSequenceSort(0,N-1);
	
	for(int i=0;i<N;i++)printf("%d ",A[i]);
}