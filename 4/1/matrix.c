#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>
#include <immintrin.h>
#include <sys/time.h>
#include "matrix.h"

#define SM (64/sizeof(double))

matrix_t *allocate_matrix(int rows, int columns)
{
	matrix_t *m = malloc(sizeof(*m));

	if (m == NULL) {
		fprintf(stderr, "allocate_matrix(): out of memory\n");
		exit(1);
	}

	strncpy(m->header, HEADER, HEADERLENGTH);
	m->rows = rows;
	m->columns = columns;
	m->data = _mm_malloc(m->rows * m->columns * sizeof(double), 64);

	if (m->data == NULL) {
		fprintf(stderr, "allocate_matrix(): out of memory\n");
		exit(1);
	}

	return m;
}

void free_matrix(matrix_t *m)
{
	free(m->data);
	free(m);
}

matrix_t *generate_random_matrix(int rows, int columns)
{
	matrix_t *m = allocate_matrix(rows, columns);

	int i, j;
	for (i = 0; i < m->rows; i++)
		for (j = 0; j < m->columns; j++)
			m->data[i * m->columns + j] =
			-1.0 + (2.0 * (double)rand() / ((double)RAND_MAX + 1.0));

	return m;
}

void show_matrix(matrix_t *m)
{
	printf("(");

	int i, j;
	for (i = 0; i < m->rows; i++) {
		printf("\n    ");
		for (j = 0; j < m->columns; j++) {
			printf("%f,", m->data[i * m->columns + j]);
		}
	}
	printf("\n)\n");
}

void compare_matrix(matrix_t *a, matrix_t *b)
{
	if (a->columns != b->columns || a->rows != b->rows) {
		printf("Dimensions differ, cannot compare.\n");
		return;
	}

	int i, j, found = 0;
	for (i = 0; i < a->rows; i++) {
		for (j = 0; j < a->columns; j++) {
			double d = fabs((double)a->data[i * a->columns + j] - (double)b->data[i * a->columns + j]);
			if (d > MAXDIFF) {
				printf("Found difference of %f at [%d,%d].\n", d, i, j);
				found=1;
			}
		}
	}
	if (!found)
		printf("Identical.\n");
}

matrix_t *load_matrix(char *path)
{
	int d = open(path, O_RDONLY);

	if (d == -1) {
		fprintf(stderr, "load_matrix(): could not open %s\n", path);
		exit(1);
	}

	char header[HEADERLENGTH];
	int rows, columns;
	ssize_t r;

	r = read(d, &header, HEADERLENGTH * sizeof(char));
	r += read(d, &rows, sizeof(int));
	r += read(d, &columns, sizeof(int));

	if (r != HEADERLENGTH * sizeof(char) + 2 * sizeof(int)) {
		fprintf(stderr, "load_matrix(): error reading %s\n", path);
		exit(1);
	}

	if (strncmp(header, HEADER, HEADERLENGTH) != 0) {
		fprintf(stderr, "load_matrix(): error reading %s: wrong header\n", path);
		exit(1);
	}

	matrix_t *m = allocate_matrix(rows, columns);

	r = read(d, m->data, m->rows * m->columns * sizeof(double));
	if (r != m->rows * m->columns * sizeof(double)) {
		fprintf(stderr, "load_matrix(): error reading %s\n", path);
		exit(1);
	}

	close(d);

	return m;
}

void store_matrix(matrix_t *m, char *path)
{
	int d = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);

	if (d == -1) {
		fprintf(stderr, "store_matrix(): could not open %s\n", path);
		exit(1);
	}

	ssize_t r;

	r = write(d, &m->header, HEADERLENGTH * sizeof(char));
	r += write(d, &m->rows, sizeof(int));
	r += write(d, &m->columns, sizeof(int));
	r += write(d, m->data, m->rows * m->columns * sizeof(double));

	if (r != HEADERLENGTH * sizeof(char) + 2 * sizeof(int) + m->rows * m->columns * sizeof(double)) {
		fprintf(stderr, "store_matrix(): error writing %s\n", path);
		exit(1);
	}

	close(d);
}

matrix_t *multiply_matrix(matrix_t *a, matrix_t *b)
{
	if (a->columns != b->rows) {
		fprintf(stderr, "multiply_matrix(): cannot multiply\n");
		exit(1);
	}

	matrix_t *c = allocate_matrix(a->rows, b->columns);

	struct timeval start, end;
	for(int i=0; i<25; i++) {
		gettimeofday(&start, NULL);
		__multiply_matrix(c, a, b);
		gettimeofday(&end, NULL);
		double exectime = ((end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec) / 1000000.0;
		double work = (a->rows * a->columns * b->columns)/1000000.0;
		work = 2*work;
		double flops = work/exectime;
		printf(" Run %d:\n", i+1);
		printf(" Work = %lf\n", work);
		printf(" Time taken is %lf seconds\n", exectime);
		printf(" Work per Second achieved is %lf GFLOPS\n\n", (flops>1000.0) ? flops/1000.0 : flops);
	}

	return c;
}

void __multiply_matrix(matrix_t *c, matrix_t *a, matrix_t *b)
{
	int i,j,k,i2,j2,k2;
	for (int i = 0; i < c->rows*c->columns; i++)
		c->data[i] = 0.0;

	/*#pragma omp parallel for default(none) shared(a,b,c) collapse(3)
	for (int i = 0; i < a->rows; i++) {
		for (int k = 0; k < a->columns; k++) {
			//c->data[i * b->columns + j] = 0.0;
			for (int j = 0; j < b->columns; j++) {
			c->data[i * b->columns + j] += a->data[i * a->columns + k] * b->data[k * b->columns + j];
			}
		}
	}*/
    /*
     * --- DEFINITION ---
     * Matrix A (M rows, N columns) x Matrix B (N rows, O columns)
     * Both matrices share the k index (range 0 .. N)
     *
     * Matrix C is then defined as c(i,j) = forall_k a(i,k) x b(k,j)
     *
     * --- PARALLELIZATION / OPENMP ---
     *  We tell the compiler the following variables will not be aliased across threads.
     */
	double *restrict ra;
	double *restrict rb;
	double *restrict rc;
    /*
     * Set the numbers of threads equal to the number of processors
     */
    //omp_set_dynamic( 0 );
    //omp_set_num_threads( omp_get_num_procs() );
    /* 
     *  default(none) -- deactivate all variable declarations
     *  shared(a,b,c) -- the matrices are available to all threads
     *  private(...)  -- indices are thread specific
     *
     *  collapse(3)   -- parallelization starts in the third for loop
     *
     *  schedule      -- way of scheduling
     */
	#pragma omp parallel for default(none) shared(a,b,c) private(i,j,k,i2,j2,k2,ra,rb,rc) collapse(3) //schedule(static, 250)
    /*
     * --- CACHE ---
     * -- ORDER / LOOP INTERCHANGE
     * From a caching perspective, what we first extract from memory is the first rows of A and B
     * Following the normal order, we would multiply the first element of B and of A. We would then
     *  throw the rest of B away and request its second row to multiply it with the second element of A
     * The efficiency of one cash request of matrix B is 1/O. Lets increase this:
     *
     * We define a component c(i,j)(k) = a(i,k) x b(k,j), so that c(i,j) = forall_k c(i,j)(k) 
     * Now we can process the structure in a way that we 
     *  first calculate forall_i,j c(i,j)(0) = a(i, 0) x b(0, j) 
     *  then forall_i,j c(i,j)(1) = a(i, 1) x b(1, j) 
     *  untill forall_i,j c(i,j)(N) = a(i, N) x b(1, N)  
     *
     */
	for (i = 0; i < a->rows; i+=SM) {
		for (j = 0; j < b->columns; j+=SM) {
			for (k = 0; k < a->columns; k+=SM) {
    /*
     * -- CACHE LINE SIZE
     * The last paragraph assumed, that the caching line size would be at least max(N, O)
     * To not require this assumption we 
     *   first query how many doubles fit into one cache line (see SM defined above)
     *   then we break our for loops further down so that every iteration takes SM elements
     *
     */
				for (i2 = 0, rc = &c->data[i*b->columns+j], ra=&a->data[i*a->columns+k]; i2 < SM; ++i2, rc+=c->columns, ra+=a->columns) {
					//_mm_prefetch (ra+8, _MM_HINT_NTA);
					for (k2=0, rb=&b->data[k*b->columns+j]; k2 < SM; ++k2, rb+=b->columns) {
						__m128d m1d = _mm_load_sd (ra+k2);
						m1d = _mm_unpacklo_pd (m1d, m1d);
						for (j2=0; j2 < SM; j2 +=2) {
							__m128d m2 = _mm_load_pd (rb+j2);
							__m128d r2 = _mm_load_pd (rc+j2);
							_mm_store_pd (rc+j2, _mm_add_pd (_mm_mul_pd (m2, m1d), r2));
						}
					}
				}
			}
		}
	}			

}
