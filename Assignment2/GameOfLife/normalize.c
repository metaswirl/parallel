//
//  normalize.c
//  GameOfLife
//
//  Created by JIANNAN GUO on 10.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//
//  **********
//  normalize the play field:
//  (1, Y)--------------------- (X, Y)
//    |                            |
//    |                            |
//    |                            |
//    |                            |
//    |                            |
//  (1, 1)--------------------- (X, 1)
//  Y = SIZE_OF_UNIT + 2
//  the purpose to let the field start from (1, 1) is to put it in the CENTER of counting field, wich start from (0, 0)
//  ( NOT because I'm not professional !!!)
//  **********

#include <stdio.h>
#include <stdlib.h>
#include "cell_struct.h"
#include "fooprint.h"

void normalize(int seq_of_process, struct cell_list * cells_to_normalize) {
    int i, len = cells_to_normalize->len;
    struct cell *cells;
    cells = cells_to_normalize->ptr;
    
    if (seq_of_process < LAST_PROCESS && seq_of_process > FIRST_PROCESS) {
        // in the middle
        for (i=0; i<len; i++) {
            cells[i].h++;
            cells[i].v -= (seq_of_process * SIZE_OF_UNIT - 2);
        }
    } else if (seq_of_process == FIRST_PROCESS) {
        // lower edge
        for (i=0; i<len; i++) {
            if (cells[i].v == SIZE_OF_FIELD - 1) {
                cells[i].v = 1;
            } else {
                cells[i].v += 2;
            }
            cells[i].h++;
        }
    } else if (seq_of_process == LAST_PROCESS) {
        // upper edge
        for (i=0; i<len; i++) {
            if (cells[i].v == 0) {
                cells[i].v = SIZE_OF_UNIT + 2;
            } else {
                cells[i].v -= (seq_of_process * SIZE_OF_UNIT -2);
            }
            cells[i].h++;
        }
    }
}