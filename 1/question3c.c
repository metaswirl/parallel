#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

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

	int const size_of_msg = 200;
	char *buf = malloc(size_of_msg * sizeof(char));

	if (rank == 0) { // first node to send
        strcpy(buf, argv[1]);
        printf("First node here copied msg '%s' to buffer '%s'\n", argv[1], buf);
    }
    MPI_Bcast( buf, size_of_msg, MPI_CHAR, 0, MPI_COMM_WORLD );
	if (rank > 0) {
		printf("%dth node, I just received %s\n", rank, buf);
    }
    printf("Sent!\n");

    MPI_Finalize();
    return 0;
}
