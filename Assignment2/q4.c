#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <mpi.h> 
 
#define N 10  // Grid size (N x N) 
#define ITERATIONS 500  // Number of time steps 
#define TEMP_SOURCE 100.0  // Heat source temperature 
 
void initialize_grid(double grid[N][N]) { 
    for (int i = 0; i < N; i++) { 
        for (int j = 0; j < N; j++) { 
            grid[i][j] = 0.0;  // Initialize all to zero 
        } 
    } 
    grid[N / 2][N / 2] = TEMP_SOURCE;  // Heat source in the center 
} 
 
void print_grid(double grid[N][N]) { 
    for (int i = 0; i < N; i++) { 
        for (int j = 0; j < N; j++) { 
            printf("%6.2f ", grid[i][j]); 
        } 
        printf("\n"); 
    } 
} 
 
int main(int argc, char *argv[]) { 
    int rank, size; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
 
    int rows_per_process = N / size;  // Split grid into rows among processes 
    double local_grid[N][N], new_grid[N][N]; 
    initialize_grid(local_grid); 
 
    MPI_Status status; 
     
    for (int iter = 0; iter < ITERATIONS; iter++) { 
        // Exchange boundary rows with neighboring processes 
        if (rank > 0) { // Send top row to above process 
            MPI_Send(local_grid[0], N, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD); 
            MPI_Recv(local_grid[-1], N, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &status); 
        } 
        if (rank < size - 1) { // Send bottom row to below process 
            MPI_Send(local_grid[rows_per_process - 1], N, MPI_DOUBLE, rank + 1, 0, 
MPI_COMM_WORLD); 
            MPI_Recv(local_grid[rows_per_process], N, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, 
&status); 
        } 
 
        // Compute new values using the finite difference method 
        for (int i = 1; i < rows_per_process - 1; i++) { 
            for (int j = 1; j < N - 1; j++) { 
                new_grid[i][j] = 0.25 * (local_grid[i - 1][j] + local_grid[i + 1][j] + 
                                         local_grid[i][j - 1] + local_grid[i][j + 1]); 
            } 
        } 
 
        // Copy the new values back to the local grid 
        for (int i = 1; i < rows_per_process - 1; i++) { 
            for (int j = 1; j < N - 1; j++) { 
                local_grid[i][j] = new_grid[i][j]; 
            } 
        } 
    } 
 
    // Gather the final grid at the root process 
    double final_grid[N][N]; 
    MPI_Gather(local_grid, rows_per_process * N, MPI_DOUBLE, final_grid, rows_per_process * N, 
MPI_DOUBLE, 0, MPI_COMM_WORLD); 
 
    // Print the final heat distribution 
    if (rank == 0) { 
        printf("Final Heat Distribution:\n"); 
        print_grid(final_grid); 
    } 
 
    MPI_Finalize(); 
return 0; 
} 
