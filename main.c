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
    
    struct cell_list *local;
    struct cell_list *next;
    local = calloc(512, sizeof(local));
    
    local = divide_field(SEQ_OF_PROCESS, &wholefield_l);
    
    printcell(local);
    
    normalize(SEQ_OF_PROCESS, local);
    
    printcell(local);
    
    h_extend(local);
    
    printcell(local);
    
    next = evolve(local);
    
    printcell(next);
    
    printcell_vividly(next);
    
    printcell_vividly(local);
    
    return 0;
}