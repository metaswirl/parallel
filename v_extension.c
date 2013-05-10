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

int v_extend(struct cell_list *local_alive_l) {
    int i, len, ex_len=0;
    struct cell *local_alive;
    
    len = local_alive_l->len;
    local_alive = local_alive_l->ptr;
    for (i=0; i<len; i++) {
        if (local_alive[i].y == SIZE_OF_FIELD - 1) {
            local_alive[len + ex_len].x = local_alive[i].x;
            local_alive[len + ex_len].y = 0;
            ex_len++;
        } else if (local_alive[i].y == 0) {
            local_alive[len + ex_len].x = local_alive[i].x;
            local_alive[len + ex_len].y = SIZE_OF_FIELD + 1;
            ex_len++;
        }
        local_alive[i].y++;
    }
    
    local_alive_l->len = len + ex_len;
    local_alive_l->ptr = local_alive;
    return 0;
}