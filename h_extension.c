//
//  v_extension.c
//  GameOfLife
//
//  Created by JIANNAN GUO on 10.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//
//  **********
//  This function take a structure of alive cell list
//  It copies the last column, appends it before the first column and vise versa.
//  The function will return 0 if succeed or -1 if an error detected.
//  **********

#include <stdio.h>
#include <stdlib.h>
#include "localize.h"
#include "cell_struct.h"
#include "fooprint.h"

int h_extend(struct cell_list *alive_cell_l) {
    int i, len, ex_len=0;
    struct cell *alive_cell;
    
    len = alive_cell_l->len;
    alive_cell = alive_cell_l->ptr;
    
    printcell(alive_cell_l, "before extend");
    
    for (i=0; i<len; i++) {
        if (alive_cell[i].h == SIZE_OF_FIELD) {
            // append last column before the first one
            alive_cell[len + ex_len].v = alive_cell[i].v;
            alive_cell[len + ex_len].h = 1;
            ex_len++;
        } else if (alive_cell[i].h == 1) {
            // append first column after the last one
            alive_cell[len + ex_len].v = alive_cell[i].v;
            alive_cell[len + ex_len].h = SIZE_OF_FIELD + 2;
            ex_len++;
        } else if (alive_cell[i].h == 0) {
            printf("ERROR in horizontal extension, please normalize first!\n");
            return 1;
        }
        alive_cell[i].h++;
    }
    
    alive_cell_l->len = len + ex_len;
    alive_cell_l->ptr = alive_cell;
    
    return 0;
}