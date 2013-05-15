//
//  msg_extract.c
//  GameOfLife
//
//  Created by JIANNAN GUO on 11.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//
//  **********
//  extract message from a given alive cells list, based on the direction of which the message will be sent to.
//  (The process of generating two direction messages can be merged. This will be performed in optimized version)
//  Please refer to cell_struct.h to check the structure of message variable.
//  **********

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cell_struct.h"

void msg_extract(char *direction, const cell_list *alive_list_l, unsigned char *msg) {
    int i, len=alive_list_l->len;
    cell *alive_list;
    alive_list = alive_list_l->ptr;
    if (!strcmp(direction, "up")) {
        for (i=0; i<len; i++) {
            if (alive_list[i].v == PROCESS_KNOWLEDGE_H - 1) {
                msg[alive_list[i].h - 1] = 1;
            }
        }
    } else if (!strcmp(direction, "down")) {
        for (i=0; i<len; i++) {
            if (alive_list[i].v == 2) {
                msg[alive_list[i].h - 1] = 1;
            }
        }
    } else {
        printf("ERROR : Illegal parameter!\n");
    }
}
