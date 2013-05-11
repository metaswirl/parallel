//
//  msg_handle.c
//  GameOfLife
//
//  Created by JIANNAN GUO on 11.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "cell_struct.h"

cell_list *msg_handle(const message msg_from_top, const message msg_from_bottom, cell_list *alive_cell_l) {
    int i, len, msg_len_top, msg_len_bottom, *absacissa_top, *absacissa_bottom;
    len = alive_cell_l->len;
    cell *alive_cell;
    alive_cell = alive_cell_l->ptr;
    msg_len_top = msg_from_top.num;
    msg_len_bottom = msg_from_bottom.num;
    absacissa_top = msg_from_top.alive_abscissa;
    absacissa_bottom = msg_from_bottom.alive_abscissa;
    
    cell_list *new_alive_cell_l;
    cell *new_alive_cell;
    new_alive_cell = calloc(PROCESS_KNOWLEDGE_H * PROCESS_KNOWLEDGE_L, sizeof(new_alive_cell));
    new_alive_cell_l = calloc(1, sizeof(new_alive_cell_l));
    
    int new_len = 0;
    
    for (i=0; i<len; i++) {
        if (alive_cell[i].v < PROCESS_KNOWLEDGE_H && alive_cell[i].v > 1) {
            new_alive_cell[new_len].h = alive_cell[i].h;
            new_alive_cell[new_len].v = alive_cell[i].h;
            new_len++;
        }
    }
    
    for (i=0; i<msg_len_top; i++) {
        new_alive_cell[new_len].h = absacissa_top[i];
        new_alive_cell[new_len].v = PROCESS_KNOWLEDGE_H;
        new_len++;
    }
    
    for (i=0; i<msg_len_bottom; i++) {
        new_alive_cell[new_len].h = absacissa_bottom[i];
        new_alive_cell[new_len].v = PROCESS_KNOWLEDGE_L;
        new_len++;
    }
    
    new_alive_cell_l->len = new_len;
    new_alive_cell_l->ptr = new_alive_cell;
    
    return new_alive_cell_l;
}