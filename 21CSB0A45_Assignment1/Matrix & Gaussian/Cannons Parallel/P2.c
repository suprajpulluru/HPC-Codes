#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define N 1000
#define NUM_THREADS 10

int** A;
int** B;
int** C;

struct thread_args{
    int** A;
    int** B;
    int** C;
    int rowStart;
    int rowEnd;
};

void shift_left(int** mat, int i, int n, int amount) {
    int* temp_buffer = (int*)malloc(sizeof(int) * n);
    for (int j = 0; j < n; j++) 
        temp_buffer[j] = mat[i][(j + amount) % n];

    for (int j = 0; j < n; j++)
        mat[i][j] = temp_buffer[j];

    free(temp_buffer);
}

void shift_up(int** mat, int j, int n, int amount) {
    int* temp_buffer = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) 
        temp_buffer[i] = mat[(i + amount) % n][j];

    for (int i = 0; i < n; i++)
        mat[i][j] = temp_buffer[i];

    free(temp_buffer);
}

void compute_product(struct thread_args *args) {
    int i, j, k;
    int** A = args->A;
    int** B = args->B;
    int** C = args->C;
    int rowStart = args->rowStart;
    int rowEnd = args->rowEnd;

    for (i = rowStart; i < rowEnd; i++) {
        shift_left(A, i, N, i);
    }

    for (j = 0; j < N; j++) {
        shift_up(B, j, N, j);
    }

    for (i = rowStart; i < rowEnd; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void cannon_mul(int** mat1, int** mat2, int** rez, int n) {
    pthread_t threads[NUM_THREADS];
    struct thread_args thread_args_array[NUM_THREADS];

    int chunk_size = N / NUM_THREADS;
    int remainder = N % NUM_THREADS;
    int start_row = 0;
    int end_row = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        start_row = end_row;
        end_row = start_row + chunk_size + (i < remainder ? 1 : 0);

        thread_args_array[i].A = mat1;
        thread_args_array[i].B = mat2;
        thread_args_array[i].C = rez;
        thread_args_array[i].rowStart = start_row;
        thread_args_array[i].rowEnd = end_row;

        pthread_create(&threads[i], NULL, (void *(*)(void *))compute_product, &thread_args_array[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main(){
    FILE* fin=fopen("Input.txt","r");
    if(!fin){
        printf("File cannot be read or written\n");
        exit(EXIT_FAILURE);
    }   

    A=(int**)(malloc((N)*sizeof(int*)));
    for(int i=0;i<N;i++){
        A[i]=(int*)(malloc((N)*sizeof(int)));
        for(int j=0;j<N;j++){
            fscanf(fin,"%d",&A[i][j]);
        }
    }

    B=(int**)(malloc((N)*sizeof(int*)));
    for(int i=0;i<N;i++){
        B[i]=(int*)(malloc((N)*sizeof(int)));
        for(int j=0;j<N;j++){
            fscanf(fin,"%d",&B[i][j]);
        }
    }    
    
    C=(int**)(malloc((N)*sizeof(int*)));
    for(int i=0;i<N;i++){
        C[i]=(int*)(malloc((N)*sizeof(int)));
        for(int j=0;j<N;j++)C[i][j]=0;
    }

    cannon_mul(A,B,C,N);

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%d ",C[i][j]);
        }
        printf("\n");
    }
}