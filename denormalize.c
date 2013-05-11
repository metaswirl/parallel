//
//  denormalize.c
//  GameOfLife
//
//  Created by JIANNAN GUO on 10.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "cell_struct.h"

cell_list *denormalize(const int seq_of_process, const cell_list *alive_cell_l) {
    int i, len, outlen = 0;
    cell_list *origin_l;
    cell *alive_cell, *origin;
    origin_l = calloc(1, sizeof(origin_l));
    origin = calloc(SIZE_OF_FIELD * SIZE_OF_UNIT, sizeof(origin));
    alive_cell = alive_cell_l->ptr;
    len = alive_cell_l->len;
    
    for (i=0; i<len; i++) {
        if (alive_cell[i].v < PROCESS_KNOWLEDGE_H && alive_cell[i].v > 1) {
            origin[outlen].v = alive_cell[i].v - 2 + SIZE_OF_UNIT * (seq_of_process - 1);
            origin[outlen].h = alive_cell[i].h - 1;
            outlen++;
        }
    }
    
    origin_l->len = outlen;
    origin_l->ptr = origin;
    
    return origin_l;
}