#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
 
#define MAX_STEPS 10  // Maximum number of steps each walker can take 
#define WALKER_ID 0   // Process 0 acts as the walker 
 
int main(int argc, char** argv) { 
    // Initialize the MPI environment 
    MPI_Init(&argc, &argv); 
 
    // Get the rank and size 
    int world_rank; 
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); 
    int world_size; 
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); 
 
    if (world_size < 2) { 
        if (world_rank == 0) { 
            printf("This program requires at least 2 processes.\n"); 
        } 
        MPI_Finalize(); 
        return 1; 
    } 
 
    int position = 0;  // Walker's position 
    int step; 
     
    if (world_rank == WALKER_ID) { 
        srand(time(NULL)); // Seed random number generator 
 
        for (int i = 0; i < MAX_STEPS; i++) { 
            // Randomly decide step: -1 (left) or +1 (right) 
            step = (rand() % 2 == 0) ? -1 : 1; 
            position += step; 
 
            printf("Walker at step %d, new position: %d\n", i + 1, position); 
 
            // Send the position to the next process 
            MPI_Send(&position, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); 
        } 
    }  
    else if (world_rank == 1) { 
        for (int i = 0; i < MAX_STEPS; i++) { 
            MPI_Recv(&position, 1, MPI_INT, WALKER_ID, 0, MPI_COMM_WORLD, 
MPI_STATUS_IGNORE); 
            printf("Observer received position: %d\n", position); 
        } 
    } 
 
    // Finalize the MPI environment 
    MPI_Finalize(); 
    return 0; 
} 
