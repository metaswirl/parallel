//
//  evolve.c
//  GameOfLife
//
//  Created by JIANNAN GUO on 10.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "cell_struct.h"
#include "fooprint.h"
#include "h_shrink.h"
#include "h_extension.h"

struct cell_list *evolve(struct cell_list *alive_cells_t) {
    int i, len = alive_cells_t->len;
    int h, v, count, x, y;
    cell *alive_cells;
    cell *next_generation;
    static cell_list *next_generation_t;
    static cell_list *next_generation_l;
    next_generation_t = calloc(1, sizeof(next_generation_t));
    next_generation_l = calloc(1, sizeof(next_generation_l));
    next_generation = calloc(512, sizeof(next_generation));
    int offset = 0;
    
    h_extend(alive_cells_t); // extend first
    
    alive_cells = alive_cells_t->ptr;
    for (i=0; i<len; i++) {
        h = alive_cells[i].h;
        v = alive_cells[i].v;
        COUNT_FIELD[v-1][h-1]++;
        COUNT_FIELD[v-1][h]++;
        COUNT_FIELD[v-1][h+1]++;
        COUNT_FIELD[v][h-1]++;
        COUNT_FIELD[v][h+1]++;
        COUNT_FIELD[v+1][h-1]++;
        COUNT_FIELD[v+1][h]++;
        COUNT_FIELD[v+1][h+1]++;
    }
    
    print_count_matrix();
    
    for (i=0; i<len; i++) {
        count = COUNT_FIELD[alive_cells[i].v][alive_cells[i].h];
        if (count == 2 || count == 3) {
            next_generation[offset].h = alive_cells[i].h;
            next_generation[offset].v = alive_cells[i].v;
            offset++;
        }
        COUNT_FIELD[alive_cells[i].v][alive_cells[i].h] = 0;
    }
    
    for (i=0; i<len; i++) {
        v = alive_cells[i].v;
        h = alive_cells[i].h;
        for (y=v-1; y<=v+1; y++) {
            for (x=h-1; x<=h+1; x++) {
                if (COUNT_FIELD[y][x] == 3) {
                    next_generation[offset].h = y;
                    next_generation[offset].v = x;
                    offset++;
                }
                COUNT_FIELD[y][x] = 0;
            }
        }
    }
    
    next_generation_t->len = offset;
    next_generation_t->ptr = next_generation;
    
    next_generation_l = shrink(next_generation_t);
    
    return next_generation_l;
}
