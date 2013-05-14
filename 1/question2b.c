#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Please declare a message to be sent along\n");
		return 1;
	}

    MPI_Init(&argc, &argv);
	
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	printf("Hello world, I am node %d of %d\n", rank, size);

	int tag = 42;
	int const size_of_msg = 200;
	
	
	if (rank == 0) { // first node to send
		printf("first node: I will send %s to %d\n", argv[1], rank+1);
		MPI_Send( argv[1], size_of_msg, MPI_CHAR, rank+1, tag, MPI_COMM_WORLD );
		printf("Sent!\n");
	} else {
		char *buf = malloc(size_of_msg * sizeof(char));
		MPI_Recv( buf, size_of_msg, MPI_CHAR, rank-1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
		printf("%dth node, I just received %s\n", rank, buf);
		if (rank+1 < size) { // not last node
			printf("%dth node: I will send %s to %d\n", rank, argv[1], rank+1);
			MPI_Send( buf, size_of_msg, MPI_CHAR, rank+1, tag, MPI_COMM_WORLD );
		} else
			printf("I am the last one :D\n");
	}

    MPI_Finalize();
    return 0;
}
