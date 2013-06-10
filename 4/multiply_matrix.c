#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "matrix.h"

int main(int argc, char **argv) {
	if (argc != 4) {
		printf("usage: %s <file A> <file B> <file C>\n"
		       "  loads both A and B and stores the product in C\n",
		       argv[0]);
		exit(1);
	}

	struct timeval start, end;

	matrix_t *a = load_matrix(argv[1]);
	matrix_t *b = load_matrix(argv[2]);

    gettimeofday(&start, NULL);
	matrix_t *c = multiply_matrix(a, b);
    gettimeofday(&end, NULL);
	double exectime = ((end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec) / 1000000.0;
	double work = (a->rows * a->columns * b->columns) / 1000000.0;
	double flops = work/exectime;
	printf(" Time taken to multiply the matrices is %lf seconds\n", exectime);
	if(flops > 1000.0)
		printf(" GFLOPS achieved when multiplying the matrices is %lf GFLOPS\n", flops/1000.0);
	else 
		printf(" MFLOPS achieved when multiplying the matrices is %lf MFLOPS\n", flops);

	store_matrix(c, argv[3]);

	free_matrix(a);
	free_matrix(b);
	free_matrix(c);
	return 0;
}
