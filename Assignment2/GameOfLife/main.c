//
//  main.c
//  dividefield
//
//  Created by JIANNAN GUO on 09.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
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

int COUNT_FIELD[COUNT_HEIGHT][COUNT_LENGTH];

int main()
{
    struct cell fullfield[10] = {
        {0,0},
        {5,5},
        {10,10},
        {20,20},
        {5,6},
        {6,7},
        {7,7},
        {63,63},
        {0,63},
        {63,0}
    };
    
    struct cell_list wholefield_l = {
        10,
        fullfield
    };
    
    printf("Process %d\n", SEQ_OF_PROCESS);
    
    cell_list *current = NULL;
    cell_list *next = NULL;
    
    message up, down;
    
    /* --------------localize-------------- */
    
    current = divide_field(SEQ_OF_PROCESS, &wholefield_l);
    
    printcell(current, "local job");

    normalize(SEQ_OF_PROCESS, current);
    
    /* --------------go through a cycle of evolution-------------- */
    
    next = evolve(current);
    
    kill_cell(current); // destroy scraps
    
    // generate the message
    
    down = msg_extract("down", next);
    
    printf("down message: num = %d\n", down.num);
    
    up = msg_extract("up", next);
    
    printf("up message: num = %d\n", up.num);
    
    /* --------------------------end------------------------------ */
    
    current = next;
    
    /* --------------go through a cycle of evolution-------------- */
    
    next = evolve(current);
    
    kill_cell(current); // destroy scraps
    
    // generate the message
    
    down = msg_extract("down", next);
    
    printf("down message: num = %d\n", down.num);
    
    up = msg_extract("up", next);
    
    printf("up message: num = %d\n", up.num);
    
    kill_msg(down);
    
    kill_msg(up);
    
    /* --------------------------end------------------------------ */
    
    /* ------------ 1000 cycles of evolution ----------- */
    int i = 0;
    current = next;
    for (i=0; i<1000; i++) {
        next = evolve(current);
        kill_cell(current);
        current = next;
    }
    /* -------------------end-------------------- */
    
    /* --------------denormalize-------------- */
    
    cell_list *origin;
    
    origin = denormalize(SEQ_OF_PROCESS, next);
    
    printcell(origin, "original coordinates");
    
    kill_cell(next);
    
    kill_cell(origin);
    
    return 0;
}