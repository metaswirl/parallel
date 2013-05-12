//
//  h_shrink.c
//  GameOfLife
//
//  Created by JIANNAN GUO on 10.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//
//  **********
//  remove first and last column and adjust the offset
//  **********

#include <stdio.h>
#include <stdlib.h>
#include "cell_struct.h"

struct cell_list * shrink(const struct cell_list *alive_cell_l) {
    int i, len = alive_cell_l->len;
    struct cell *before_shrink;
    struct cell *after_shrink;
    struct cell_list *after_shrink_l;
    after_shrink_l = malloc(sizeof(after_shrink_l));
    after_shrink = calloc(PROCESS_KNOWLEDGE_H * PROCESS_KNOWLEDGE_L, sizeof(after_shrink));
    before_shrink = alive_cell_l->ptr;
    int len_after_shrink = 0;
    
    for (i=0; i<len; i++) {
        if (before_shrink[i].h >= 2 && before_shrink[i].h <= SIZE_OF_FIELD+1) {
            after_shrink[len_after_shrink].h = before_shrink[i].h - 1;
            after_shrink[len_after_shrink].v = before_shrink[i].v;
            len_after_shrink++;
        }
    }
    
    after_shrink_l->len = len_after_shrink;
    after_shrink_l->ptr = after_shrink;
    
    return after_shrink_l;
}