#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h> 
#define N 100  // Total number of elements 
int main(int argc, char *argv[]) { 
int rank, size; 
MPI_Init(&argc, &argv); 
MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
MPI_Comm_size(MPI_COMM_WORLD, &size); 
int local_N = N / size;  // Divide array equally among processes 
int local_sum = 0, global_sum = 0; 
int *data = NULL; 
// Only the root process initializes the full array 
if (rank == 0) { 
data = (int *)malloc(N * sizeof(int)); 
for (int i = 0; i < N; i++) 
data[i] = i + 1;  // Initialize array with values 1 to N 
} 
// Each process gets a chunk of data 
int *local_data = (int *)malloc(local_N * sizeof(int)); 
MPI_Scatter(data, local_N, MPI_INT, local_data, local_N, MPI_INT, 0, MPI_COMM_WORLD); 
    // Compute local sum 
    for (int i = 0; i < local_N; i++) 
        local_sum += local_data[i]; 
 
    // Perform reduction to sum up all local sums 
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); 
 
    // Root process prints the result 
    if (rank == 0) { 
        printf("Parallel Reduction: Sum = %d\n", global_sum); 
        printf("Expected Sum = %d\n", (N * (N + 1)) / 2);  // Formula for sum of first N natural numbers 
        free(data); 
    } 
 
    free(local_data); 
    MPI_Finalize(); 
    return 0; 
} 
