#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h> 
 
#define N 100  // Total number of elements 
 
int main(int argc, char *argv[]) { 
    int rank, size; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
 
    int local_N = N / size;  // Divide the workload 
    double local_dot = 0.0, global_dot = 0.0; 
    double *A = NULL, *B = NULL; 
 
    // Root process initializes the vectors 
    if (rank == 0) { 
        A = (double *)malloc(N * sizeof(double)); 
        B = (double *)malloc(N * sizeof(double)); 
        for (int i = 0; i < N; i++) { 
            A[i] = i + 1;  // Example: 1, 2, 3, ..., N 
            B[i] = (i + 1) * 2;  // Example: 2, 4, 6, ..., 2N 
        } 
    } 
 
    // Allocate memory for local parts of A and B 
    double *local_A = (double *)malloc(local_N * sizeof(double)); 
    double *local_B = (double *)malloc(local_N * sizeof(double)); 
 
    // Scatter the vectors to all processes 
    MPI_Scatter(A, local_N, MPI_DOUBLE, local_A, local_N, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
    MPI_Scatter(B, local_N, MPI_DOUBLE, local_B, local_N, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
 
    // Compute the local dot product 
    for (int i = 0; i < local_N; i++) { 
        local_dot += local_A[i] * local_B[i]; 
    } 
 
    // Perform reduction to sum up all local dot products 
    MPI_Reduce(&local_dot, &global_dot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
 
    // Root process prints the result 
    if (rank == 0) { 
        printf("Parallel Dot Product: %f\n", global_dot); 
        // Compute expected value for verification 
        double expected_dot = 0.0; 
        for (int i = 0; i < N; i++) { 
            expected_dot += (i + 1) * ((i + 1) * 2); 
        } 
        printf("Expected Dot Product: %f\n", expected_dot); 
        free(A); 
        free(B); 
    } 
 
    // Free allocated memory 
    free(local_A); 
    free(local_B); 
 
    MPI_Finalize(); 
    return 0; 
}
