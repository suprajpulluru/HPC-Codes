#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10

struct thread_args{
	int **A;
	int **B;
	int r1;
	int c1;
	int r2;
	int c2;
	int** result;
};

void *multiply(void *args){
	struct thread_args *arg = (struct thread_args *)args;
	int **A = arg->A;
	int **B = arg->B;
	int r1 = arg->r1;
	int r2 = arg->r2;
	int c1 = arg->c1;
	int c2 = arg->c2;

	int **result = malloc((N / 2) * sizeof(int *));
	for (int i=r1;i<r1+N/2;i++){
		result[i-r1] = malloc((N/2) * sizeof(int));
		for(int j=c2;j<c2+N/2;j++){
			result[i-r1][j-c2]=0;
			for(int k=0;k<N/2;k++)result[i-r1][j-c2] += A[i][c1+k] * B[r2+k][j];
		}
	}
	arg->result = result;

	pthread_exit(NULL);
}

void add(int **A, int **B, int **C, int row, int col){
	for (int i=0;i<N/2;i++){
		for (int j=0;j<N/2;j++){
			C[row+i][col+j] += A[i][j]+B[i][j];
		}
	}
}

int main(){
	FILE* fin=fopen("Input.txt","r");
	if(!fin){
		printf("File cannot be read\n");
		exit(EXIT_FAILURE);
	}	
	
	int** A = malloc((N)*sizeof(int*));
	for(int i=0;i<N;i++){
		A[i] = malloc((N)*sizeof(int));
		for(int j=0;j<N;j++)fscanf(fin,"%d",&A[i][j]);
	}

	int** B = malloc((N)*sizeof(int*));
	for(int i=0;i<N;i++){
		B[i] = malloc((N)*sizeof(int));
		for(int j=0;j<N;j++)fscanf(fin,"%d",&B[i][j]);
	}
	
	int** C = malloc((N)*sizeof(int*));
	for(int i=0;i<N;i++){
		C[i] = malloc((N)*sizeof(int));
		for(int j=0;j<N;j++)C[i][j]=0;
	}

	int offsets[8][4] = {
		{0,0,0,0},
		{0,0,0,N/2},
		{0,N/2,N/2,0},
		{0,N/2,N/2,N/2},
		{N/2,0,0,0},
		{N/2,0,0,N/2},
		{N/2,N/2,N/2,0},
		{N/2,N/2,N/2,N/2}
	};

	pthread_t tid[8];
	struct thread_args args[8];

	for(int i=0;i<8;i++){
		args[i].A = A;
		args[i].B = B;
		args[i].r1 = offsets[i][0];
		args[i].c1 = offsets[i][1];
		args[i].r2 = offsets[i][2];
		args[i].c2 = offsets[i][3];
		pthread_create(&tid[i], NULL, multiply, (void *)&args[i]);
	}

	for(int i=0;i<8;i++)pthread_join(tid[i], NULL);

	int **A11B11 = args[0].result;
	int **A11B12 = args[1].result;
	int **A12B21 = args[2].result;
	int **A12B22 = args[3].result;
	int **A21B11 = args[4].result;
	int **A21B12 = args[5].result;
	int **A22B21 = args[6].result;
	int **A22B22 = args[7].result;

	add(A11B11, A12B21, C, 0, 0);
	add(A11B12, A12B22, C, 0, N / 2);
	add(A21B11, A22B21, C, N / 2, 0);
	add(A21B12, A22B22, C, N / 2, N / 2);

	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++)printf("%d ",C[i][j]);
		printf("\n");
	}
}