#include <stdio.h>
#include <stdlib.h>

#define N 10

int** A;
int** B;
int** C;

int** multiply(int** A,int** B,int ARowStart,int AColStart,int BRowStart,int BColStart){
	int** result = malloc((N/2)*sizeof(int*));
	for(int i=ARowStart;i<ARowStart+N/2;i++){
		result[i-ARowStart] = malloc((N/2)*sizeof(int));
		for(int j=BColStart;j<BColStart+N/2;j++){
			result[i-ARowStart][j-BColStart]=0;
			for(int k=0;k<N/2;k++)result[i-ARowStart][j-BColStart]+=A[i][AColStart+k]*B[BRowStart+k][j];
		}	
	}
	return result;
}

void add(int** A,int** B,int** C,int CRowStart,int CColStart){
	for(int i=0;i<N/2;i++){
		for(int j=0;j<N/2;j++){
			C[CRowStart+i][CColStart+j]+=A[i][j]+B[i][j];
		}
	}	
}

int main(){
	FILE* fin = fopen("Input.txt","r");
	if(!fin){
		printf("File cannot be read\n");
		exit(EXIT_FAILURE);
	}	
	
	A = malloc((N)*sizeof(int*));
	for(int i=0;i<N;i++){
		A[i] = malloc((N)*sizeof(int));
		for(int j=0;j<N;j++)fscanf(fin,"%d",&A[i][j]);
	}

	B = malloc((N)*sizeof(int*));
	for(int i=0;i<N;i++){
		B[i] = malloc((N)*sizeof(int));
		for(int j=0;j<N;j++)fscanf(fin,"%d",&B[i][j]);
	}
	
	C = malloc((N)*sizeof(int*));
	for(int i=0;i<N;i++){
		C[i] = malloc((N)*sizeof(int));
		for(int j=0;j<N;j++)C[i][j]=0;
	}
	
	int** A11B11 = multiply(A,B,0,0,0,0);
	int** A11B12 = multiply(A,B,0,0,0,N/2);
	int** A12B21 = multiply(A,B,0,N/2,N/2,0);
	int** A12B22 = multiply(A,B,0,N/2,N/2,N/2);	
	int** A21B11 = multiply(A,B,N/2,0,0,0);
	int** A21B12 = multiply(A,B,N/2,0,0,N/2);
	int** A22B21 = multiply(A,B,N/2,N/2,N/2,0);
	int** A22B22 = multiply(A,B,N/2,N/2,N/2,N/2);
	
	
	add(A11B11,A12B21,C,0,0);
	add(A11B12,A12B22,C,0,N/2);
	add(A21B11,A22B21,C,N/2,0);
	add(A21B12,A22B22,C,N/2,N/2);
	
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++)printf("%d ",C[i][j]);
		printf("\n");
	}
}