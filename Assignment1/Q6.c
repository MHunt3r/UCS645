#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
 
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
 
    int number_amount; 
 
    if (world_rank == 0) { 
        const int MAX_NUMBERS = 100; 
        int numbers[MAX_NUMBERS]; 
 
        // Initialize random seed 
        srand(time(NULL)); 
 
        // Generate a random number of elements to send (between 1 and MAX_NUMBERS) 
        number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS; 
 
        // Fill the array with random values 
        for (int i = 0; i < number_amount; i++) { 
            numbers[i] = rand() % 100;  // Random numbers between 0 and 99 
        } 
 
        // Send the numbers to Process 1 
        MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD); 
        printf("Process 0 sent %d numbers to Process 1\n", number_amount); 
    }  
    else if (world_rank == 1) { 
        MPI_Status status; 
 
        // Probe for an incoming message from Process 0 
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status); 
 
        // Get the message size 
        MPI_Get_count(&status, MPI_INT, &number_amount); 
 
        // Dynamically allocate a buffer for the incoming numbers 
        int* number_buf = (int*)malloc(sizeof(int) * number_amount); 
 
        // Receive the message using the allocated buffer 
        MPI_Recv(number_buf, number_amount, MPI_INT, 0, 0, MPI_COMM_WORLD, 
MPI_STATUS_IGNORE); 
        printf("Process 1 dynamically received %d numbers from Process 0.\n", number_amount); 
 
        // Free the allocated memory 
        free(number_buf); 
    } 
 
    // Finalize the MPI environment 
    MPI_Finalize(); 
    return 0; 
} 
