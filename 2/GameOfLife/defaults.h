#ifndef LIFE_DEFAULTS_H
#define LIFE_DEFAULTS_H

#include <stddef.h>
#include <stdbool.h>
#include <getopt.h>

static const char * opts = "c:r:g:i:o::xh?";
static const struct option long_opts[] = {
	{ "columns", required_argument, NULL, 'c' },
	{ "rows", required_argument, NULL, 'r' },
	{ "gens", required_argument, NULL, 'g' },
	{ "output", required_argument, NULL, 'o' },
	{ "input", required_argument, NULL, 'i' },
	{ "help", no_argument, NULL, 'h' },
	{ NULL, no_argument, NULL, 0 }
};

// Default parameters for the simulation
const int     DEFAULT_ROWS = 50;
const int     DEFAULT_COLS = 100;
const int     DEFAULT_GENS = 100;

struct args_t {
	int nrows;
	int ncols;
	int ngens;
	char *infile;
	char *outfile;
};

enum CELL_STATES {
	DEAD = 0,
	ALIVE
};

// Cells become DEAD with more than UPPER_THRESH 
// or fewer than LOWER_THRESH neighbors
const int UPPER_THRESH = 3;
const int LOWER_THRESH = 2;

// Cells with exactly SPAWN_THRESH neighbors become ALIVE
const int SPAWN_THRESH = 3;

#endif
