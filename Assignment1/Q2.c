#include <mpi.h> 
#include <stdio.h> 
 
int main(int argc, char** argv) { 
    // Initialize the MPI environment 
    MPI_Init(&argc, &argv); 
 
    // Get the rank and size 
    int world_rank; 
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); 
    int world_size; 
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); 
 
    // Ensure we have at least 2 processes 
    if (world_size < 2) { 
        if (world_rank == 0) { 
            printf("This program requires at least 2 processes.\n"); 
        } 
        MPI_Finalize(); 
        return 1; 
    } 
 
    int number; 
    if (world_rank == 0) { 
        // Process 0 sends a number to Process 1 
        number = -1; 
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); 
        printf("Process 0 sent number %d to process 1\n", number); 
    } else if (world_rank == 1) { 
        // Process 1 receives the number from Process 0 
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        printf("Process 1 received number %d from process 0\n", number); 
    } 
 
    // Finalize the MPI environment 
    MPI_Finalize(); 
    return 0; 
} 
