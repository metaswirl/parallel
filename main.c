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
#include "h_extension.h"
#include "normalize.h"
#include "fooprint.h"
#include "evolve.h"
#include "h_shrink.h"
#include "msg_extract.h"
#include "denormalize.h"

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
    
    printf("%ld\n", sizeof(wholefield_l));
    
    struct cell_list *current;
    struct cell_list *next;
    struct cell_list *shrinked;
    
    current = calloc(512, sizeof(current));
    
    current = divide_field(SEQ_OF_PROCESS, &wholefield_l);
    
    printcell(current, "after divide_field");

    normalize(SEQ_OF_PROCESS, current);
    
    h_extend(current);
    
    next = evolve(current);
    
    shrinked = shrink(next);
    
    printcell(shrinked, "after shrink");
    
    h_extend(shrinked);
    
    printcell(shrinked, "after extend");
    
    printcell_vividly(shrinked, "extend next");
    
    cell_list *next_next = evolve(next);
    
    printcell(next_next, "next next generation");
    
    printcell_vividly(next_next, "next next generation");
    
    shrinked = shrink(next_next);
    
    printcell(shrinked, "after shrink");
    
    message down = msg_extract("down", shrinked);
    
    printf("down message: num = %d\n", down.num);
    
    message up = msg_extract("up", shrinked);
    
    printf("up message: num = %d\n", up.num);
    
    cell_list *origin;
    
    origin = denormalize(SEQ_OF_PROCESS, shrinked);
    
    printcell(origin, "original coordinates");
    
    return 0;
}