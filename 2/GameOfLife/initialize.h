#ifndef LIFE_H
#define LIFE_H


#include "defaults.h" // For Life's constants

#include <stdlib.h>   // For malloc et al.
#include <stdbool.h>  // For true/false
#include <getopt.h>   // For argument processing
#include <stdio.h>    // For file i/o


void   init_grids (struct cell_list * world, char * infile);
void   write_grid (struct cell_list * world, char *outfile);
void   parse_args (struct args_t * args, int argc, char ** argv);
void   usage ();

/*
	Initialize cells based on input file, otherwise all cells
	are DEAD.
*/
void init_grids (struct cell_list * world, char * infile) {
	FILE * fd;
	int i,j,index=0;

	world->len = (int)calloc(1, sizeof(world->len));

	if (infile != NULL) {
		if ((fd = fopen(infile, "r")) == NULL) {
			perror("Failed to open file for input");
			exit(EXIT_FAILURE);
		}

		if (fscanf(fd, "%d\n", &world->len) == EOF) {
			printf("File must at least define grid dimensions!\nExiting.\n");
			exit(EXIT_FAILURE);
		}
	}

	world->ptr = (struct cell *) calloc((world->len), sizeof(struct cell));

	if (infile != NULL) {
		while (fscanf(fd, "%d %d\n", &i, &j) != EOF) {
			world->ptr[index].h = i;
			world->ptr[index].v = j;
			index++;
		}
		
		fclose(fd);
	}
}

/*
		Dumps the current state of the world to outfile.
		Only outputs the coordinates of ALIVE cells.
*/
void write_grid (struct cell_list *world, char *outfile) {
	FILE * fd;
	int i;

	if (outfile != NULL) {
		if ((fd = fopen(outfile, "w")) == NULL) {
			perror("Failed to open file for output");
			exit(EXIT_FAILURE);
		}

		for (i = 0; i < world->len; i++) {
			fprintf(fd, "%d %d\n", world->ptr[i].h, world->ptr[i].v);
		}

		fclose(fd);
	}
}

/*
	usage()
		Describes Life's command line option
*/
void usage () {
	printf("\nUsage: Life [options]\n");
	printf("  -c|--columns <number>   Number of columns in grid. Default: %d\n", DEFAULT_COLS);
	printf("  -r|--rows <number>      Number of rows in grid. Default: %d\n", DEFAULT_ROWS);
	printf("  -g|--gens <number>      Number of generations to run. Default: %d\n", DEFAULT_GENS);
	printf("  -i|--input <filename>   Input pattern file.\n");
	printf("  -o|--output <filename>  Output file.\n");

	exit(EXIT_FAILURE);
}

/*
		Parse command line arguments
*/
void parse_args (struct args_t * args, int argc, char ** argv) {
	int opt       = 0;
	int opt_index = 0;
	for (;;) {
		opt = getopt_long(argc, argv, opts, long_opts, &opt_index);

		if (opt == -1) break;

		switch (opt) {
			case 'c':
				args->ncols = strtol(optarg, (char**) NULL, 10);
				break;
			case 'r':
				args->nrows = strtol(optarg, (char**) NULL, 10);
				break;
			case 'g':
				args->ngens = strtol(optarg, (char**) NULL, 10);
				break;
			case 'i':
				args->infile = optarg;
				break;
			case 'o':
				args->outfile = optarg;
				break;
			case 'h':
			case '?':
				usage();
				break;

			default:
				break;
		}
	}
}

#endif
