//  Created by JIANNAN GUO on 11.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//
//  **********
//  Distribute the job based on sequence number of current process(node).
//  return type: cell_list
//  NOTICE: the first node will contain the alive cells in last row and vise versa.
//  **********

#include <stdio.h>
#include <stdlib.h>
#include "localize.h"
#include "cell_struct.h"

struct cell_list * divide_field(int seq_of_process, const struct cell_list *full_alive_l) {
    int i, length_in, length_out=0;
    struct cell *alive_list;
    int upper_border, lower_border;
    length_in = full_alive_l->len;
    alive_list = full_alive_l->ptr;
    struct cell *localized_list;
    struct cell_list *localized_list_t;
    localized_list_t = calloc(1, sizeof(localized_list_t));
    localized_list = calloc(PROCESS_KNOWLEDGE_H * PROCESS_KNOWLEDGE_L, sizeof(localized_list));
    // determine the edges on both sides
    if (seq_of_process < LAST_PROCESS && seq_of_process > FIRST_PROCESS) {
        // not edge, process in the center area.
        upper_border = SIZE_OF_UNIT * (seq_of_process);
        lower_border = SIZE_OF_UNIT * (seq_of_process - 1) - 1;
        for(i=0; i<length_in; i++) {
            if(alive_list[i].v <= upper_border && alive_list[i].v >= lower_border) {
                localized_list[length_out].h = alive_list[i].h;
                localized_list[length_out].v = alive_list[i].v;
                length_out++;
            }
        }
    } else if (seq_of_process == FIRST_PROCESS) {
        // lower edge process.
        upper_border=SIZE_OF_UNIT;
        for(i=0; i<length_in; i++) {
            if((alive_list[i].v >= 0 && alive_list[i].v <= upper_border) || (alive_list[i].v == SIZE_OF_FIELD - 1)) {
                localized_list[length_out].h = alive_list[i].h;
                localized_list[length_out].v = alive_list[i].v;
                length_out++;
            }
        }
    } else if (seq_of_process == LAST_PROCESS) {
        // upper edge process.
        lower_border=SIZE_OF_FIELD - SIZE_OF_UNIT;
        for(i=0; i<length_in; i++) {
            if((alive_list[i].v >= lower_border && alive_list[i].v <= SIZE_OF_FIELD - 1) || (alive_list[i].v == 0)) {
                localized_list[length_out].h = alive_list[i].h;
                localized_list[length_out].v = alive_list[i].v;
                length_out++;
            }
        }
    }
    
    localized_list_t->len = length_out;
    localized_list_t->ptr = localized_list;
    
    return localized_list_t;
}



