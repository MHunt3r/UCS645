#include <mpi.h> 
#include <stdio.h> 
 
#define PING_PONG_LIMIT 10  // Define the maximum ping-pong count 
 
int main(int argc, char** argv) { 
    // Initialize the MPI environment 
    MPI_Init(&argc, &argv); 
 
    // Get the rank and size 
    int world_rank; 
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); 
    int world_size; 
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); 
 
    // Ensure there are exactly 2 processes 
    if (world_size != 2) { 
        if (world_rank == 0) { 
            printf("This program requires exactly 2 processes.\n"); 
        } 
        MPI_Finalize(); 
        return 1; 
    } 
 
    int ping_pong_count = 0; 
    int partner_rank = (world_rank + 1) % 2; 
 
    while (ping_pong_count < PING_PONG_LIMIT) { 
        if (world_rank == ping_pong_count % 2) { 
            // Increment the count before sending 
            ping_pong_count++; 
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD); 
            printf("Process %d sent and incremented ping_pong_count %d to process %d\n", 
                   world_rank, ping_pong_count, partner_rank); 
        } else { 
            MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, 
MPI_STATUS_IGNORE); 
            printf("Process %d received ping_pong_count %d from process %d\n", 
                   world_rank, ping_pong_count, partner_rank); 
        } 
    } 
 
    // Finalize the MPI environment 
    MPI_Finalize(); 
    return 0; 
}
