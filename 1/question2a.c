#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
	
    int rank, size;

    printf("Hello world :)\n");	
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("My rank in the communicator? Well that would be %d\n", rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    printf("The number of my group mates? Good question, that would be %d\n", size);
    printf("I am happy that I could be of service!\n");

    MPI_Finalize();
    return 0;
}
