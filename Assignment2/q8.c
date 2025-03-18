#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h> 
 
#define N 4 // Matrix size (N x N) 
 
void printMatrix(int *matrix, int rows, int cols, const char *msg) { 
    printf("%s\n", msg); 
    for (int i = 0; i < rows; i++) { 
        for (int j = 0; j < cols; j++) { 
            printf("%3d ", matrix[i * cols + j]); 
        } 
        printf("\n"); 
    } 
} 
 
int main(int argc, char *argv[]) { 
    int rank, size; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
 
    int local_N = N / size; // Rows handled per process 
    int *A = NULL, *B = NULL; 
    int *local_A = (int *)malloc(local_N * N * sizeof(int)); 
    int *local_B = (int *)malloc(local_N * N * sizeof(int)); 
 
    if (rank == 0) { 
        // Allocate and initialize the full matrix 
        A = (int *)malloc(N * N * sizeof(int)); 
        B = (int *)malloc(N * N * sizeof(int)); 
        for (int i = 0; i < N; i++) 
            for (int j = 0; j < N; j++) 
                A[i * N + j] = i * N + j + 1; // Example: 1, 2, 3, ... 
 
        printMatrix(A, N, N, "Original Matrix A:"); 
    } 
 
    // Scatter rows of A to all processes 
    MPI_Scatter(A, local_N * N, MPI_INT, local_A, local_N * N, MPI_INT, 0, MPI_COMM_WORLD); 
 
    // Transpose locally 
    for (int i = 0; i < local_N; i++) 
        for (int j = 0; j < N; j++) 
            local_B[j * local_N + i] = local_A[i * N + j]; 
 
    // Gather transposed blocks into B 
    MPI_Gather(local_B, local_N * N, MPI_INT, B, local_N * N, MPI_INT, 0, MPI_COMM_WORLD); 
 
    // Print transposed matrix in root 
    if (rank == 0) { 
        printMatrix(B, N, N, "Transposed Matrix B:"); 
        free(A); 
        free(B); 
    } 
 
    free(local_A); 
    free(local_B); 
    MPI_Finalize(); 
    return 0; 
}
