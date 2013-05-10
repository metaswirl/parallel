//
//  v_extension.c
//  GameOfLife
//
//  Created by JIANNAN GUO on 10.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "localize.h"
#include "cell_struct.h"

int h_extend(struct cell_list *local_alive_l) {
    int i, len, ex_len=0;
    struct cell *local_alive;
    
    len = local_alive_l->len;
    local_alive = local_alive_l->ptr;
    for (i=0; i<len; i++) {
        if (local_alive[i].h == SIZE_OF_FIELD) {
            // append last column before the first one
            local_alive[len + ex_len].v = local_alive[i].v;
            local_alive[len + ex_len].h = 1;
            ex_len++;
        } else if (local_alive[i].h == 1) {
            // append first column after the last one
            local_alive[len + ex_len].v = local_alive[i].v;
            local_alive[len + ex_len].h = SIZE_OF_FIELD + 2;
            ex_len++;
        } else if (local_alive[i].h == 0) {
            printf("ERROR in horizontal extension, please normalize first!\n");
            return 1;
        }
        local_alive[i].h++;
    }
    
    local_alive_l->len = len + ex_len;
    local_alive_l->ptr = local_alive;
    
    return 0;
}