//
//  msg_handle.c
//  GameOfLife
//
//  Created by JIANNAN GUO on 11.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//
//  **********
//  take the message and extract the information. Copy it into alive list at the same time
//  **********

#include <stdio.h>
#include <stdlib.h>
#include "cell_struct.h"
#include "killer.h"

cell_list *msg_handle(unsigned char *msg_from_top, unsigned char *msg_from_bottom, cell_list *alive_cell_l) {
    int i, len;
    len = alive_cell_l->len;
    cell *alive_cell;
    alive_cell = alive_cell_l->ptr;
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
    
    for (i=0; i<SIZE_OF_FIELD; i++) {
        if (msg_from_top[i] == 1) {
            new_alive_cell[new_len].h = i + 1;
            new_alive_cell[new_len].v = PROCESS_KNOWLEDGE_H;
            new_len++;
        }
    }
    
    for (i=0; i<SIZE_OF_FIELD; i++) {
        if (msg_from_bottom[i] == 1) {
            new_alive_cell[new_len].h = i + 1;
            new_alive_cell[new_len].v = 1;
            new_len++;
        }
    }
    
    new_alive_cell_l->len = new_len;
    new_alive_cell_l->ptr = new_alive_cell;
    
    kill_cell(alive_cell_l);
    
    return new_alive_cell_l;
}
