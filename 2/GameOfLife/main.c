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
#include "localize.h"
#include "cell_struct.h"
//#include "h_extension.h"
#include "normalize.h"
#include "fooprint.h"
#include "evolve.h"
//#include "h_shrink.h"
#include "msg_extract.h"
#include "denormalize.h"
#include "killer.h"
#include "defaults.h"
#include "initialize.h"
#include <mpi.h>
#include <getopt.h>

int COUNT_FIELD[COUNT_HEIGHT][COUNT_LENGTH];

int main(int argc, char ** argv) {

        struct args_t args = {DEFAULT_ROWS, DEFAULT_COLS, DEFAULT_GENS, NULL, NULL};
        struct cell_list wholefield_l;
        int rank, size,i=0;

/*    struct cell fullfield[10] = {
        {4,2},
        {4,3},
        {5,2},
        {5,3},
        {6,4},
        {6,5},
        {7,4},
        {7,5},
        {7,20},
        {63,0}
    };
    
    struct cell_list wholefield_l = {
        10,
        fullfield
    }; */
    
    //printf("Process %d\n", SEQ_OF_PROCESS);

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        parse_args(&args, argc, argv);

        printf("The arguments are:\n%d, %d, %d, %s, %s\n", args.nrows, args.ncols, args.ngens, args.infile, args.outfile);

        init_grids(&wholefield_l, args.infile);

        printf("\nThe number of cells initially are: %d\n", wholefield_l.len);
        for(i=0; i<wholefield_l.len; i++)
                printf("%d %d\n", wholefield_l.ptr[i].h, wholefield_l.ptr[i].v);
    
//    cell_list *current = NULL;
//    cell_list *next = NULL;
//    
//    message *up, *down;
//    
//    /* --------------localize-------------- */
//    
//    current = divide_field(SEQ_OF_PROCESS, &wholefield_l);
//
//    normalize(SEQ_OF_PROCESS, current);
//    
//    printcell(current, "local job");
//    
//    /* --------------go through a cycle of evolution-------------- */
//    
//    next = evolve(current);
//    
//    printcell(next, "1st generation");
//    
//    printcell_vividly(next, "1st generation");
//    
//    // generate the message
//    
//    down = msg_extract("down", next);
//    
//    printf("down message: num = %d\n", down->num);
//    
//    up = msg_extract("up", next);
//    
//    printf("up message: num = %d\n", up->num);
//    
//    kill_msg(down);
//    
//    kill_msg(up);
//    
//    /* --------------------------end------------------------------ */
//    
//    current = next;
//    
//    /* --------------go through a cycle of evolution-------------- */
//    
//    next = evolve(current);
//    
//    printcell(next, "2nd generation");
//    
//    printcell_vividly(next, "2nd generation");
//    
//    // generate the message
//    
//    down = msg_extract("down", next);
//    
//    printf("down message: num = %d\n", down->num);
//    
//    up = msg_extract("up", next);
//    
//    printf("up message: num = %d\n", up->num);
//    
//    kill_msg(down);
//    
//    kill_msg(up);
//    
//    /* --------------------------end------------------------------ */
//    
//    current = next;
//    
//    /* ------------ for memory debugger ----------- */
//    /*
//    while (1) {
//        next = evolve(current);
//        kill_cell(current);
//        current = next;
//    }
//    */
//    
//    for (int i=0; i<1000000; i++) {
//        next = evolve(current);
//        current = next;
//    }
//    /* -------------------end-------------------- */
//    
//    /* --------------denormalize-------------- */
//    
//    cell_list *origin;
//    
//    origin = denormalize(SEQ_OF_PROCESS, next);
//    
//    printcell(origin, "coordinates in standard format");
//    
//    kill_cell(next);
//    
//    kill_cell(origin);
    
    return 0;
}
