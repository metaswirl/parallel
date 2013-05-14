//
//  main.c
//  dividefield
//
//  Created by JIANNAN GUO on 09.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <getopt.h>
#include <stdbool.h>
#include "localize.h"
#include "cell_struct.h"
//#include "h_extension.h"
#include "normalize.h"
#include "fooprint.h"
#include "evolve.h"
//#include "h_shrink.h"
#include "message.h"
#include "denormalize.h"
#include "killer.h"
#include "defaults.h"
#include "initialize.h"

int COUNT_FIELD[COUNT_HEIGHT][COUNT_LENGTH];

int main(int argc, char ** argv) {

	struct args_t args = {DEFAULT_ROWS, DEFAULT_COLS, DEFAULT_GENS, NULL, NULL};
	struct cell_list wholefield_l;
	int rank, size,i=0, ncols, ngens, nrows;

    //printf("Process %d\n", SEQ_OF_PROCESS);

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	parse_args(&args, argc, argv);
	ncols=args.ncols; nrows=args.nrows; ngens=args.ngens;

	init_grids(&wholefield_l, args.infile);
//	printf("\nThe number of cells initially are: %d\n", wholefield_l.len);
//	for(i=0; i<wholefield_l.len; i++)
//		printf("%d %d\n", wholefield_l.ptr[i].h, wholefield_l.ptr[i].v);

	cell_list *current = NULL;
	cell_list *next = NULL;
	unsigned char upsend[ncols], downsend[ncols];
	unsigned char uprecv[ncols], downrecv[ncols];

	enum TAGS { TOP, BOT };

	//-------------localize--------------
	current = divide_field(rank+1, &wholefield_l);
	if(rank == 1)
		printcell(current, "0th generation - current");
	normalize(rank+1, current);
//	printcell(current, "local job");

	//-------------The loop for the number of generations----------

	//if(rank == 1)
	//	printcell(current, "0th generation - current");
	//for(i=0; i<ngens; i++) {
		// --------------go through a cycle of evolution-----------
		next = evolve(current);
		if(rank == 1)
			printcell(next, "1st generation");
		//	printcell_vividly(next, "1st generation");

		// generate the message
		msg_extract("down", next, downsend);
		//	printf("down message: num = %d\n", down->num);
		msg_extract("up", next, upsend);
		//	printf("up message: num = %d\n", up->num);

		MPI_Status status;
		int top_rank  = (rank-1+size) % size;
		int bot_rank = (rank+1) % size;

		if (size != 1) {
			// copy sides to neighboring processes
			MPI_Sendrecv(upsend, ncols, MPI_CHAR, top_rank, TOP,
				downrecv, ncols, MPI_CHAR, bot_rank, TOP,
				MPI_COMM_WORLD, &status);

			MPI_Sendrecv(downsend, ncols, MPI_CHAR, bot_rank,
				BOT, uprecv, ncols, MPI_CHAR, top_rank,
				BOT, MPI_COMM_WORLD, &status);
		}

		

		kill_msg(downsend);
		kill_msg(upsend);

		//--------------------------end------------------------------
		current = next;
		if(rank == 1)
			printcell(next, "2nd generation");

	//}


	// ------------ for memory debugger ----------- */
	/*
	while (1) {
		next = evolve(current);
		kill_cell(current);
		current = next;
	}

	for (int i=0; i<1000000; i++) {
		next = evolve(current);
		current = next;
	}*/

	// -------------------end-------------------- */
	// --------------denormalize-------------- */

	cell_list *origin;
	origin = denormalize(rank+1, next);
	if(rank == 1 || rank == 2)
		printcell(origin, "coordinates in standard format");
	kill_cell(next);
	kill_cell(origin);

	MPI_Finalize();
    
    return 0;
}
