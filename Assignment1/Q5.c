#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
 
#define MAX_NUMBERS 100  // Maximum number of integers to send 
 
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
 
    int numbers[MAX_NUMBERS];  // Array to store numbers 
    int number_amount;         // Actual count of numbers sent/received 
 
    if (world_rank == 0) { 
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
         
        // Receive at most MAX_NUMBERS from Process 0 
        MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 
 
        // Determine how many numbers were actually received 
        MPI_Get_count(&status, MPI_INT, &number_amount); 
 
        // Print the received information 
        printf("Process 1 received %d numbers from Process 0. Message source = %d, tag = 
%d\n", 
               number_amount, status.MPI_SOURCE, status.MPI_TAG); 
    } 
 
    // Finalize the MPI environment 
    MPI_Finalize(); 
    return 0; 
} 
