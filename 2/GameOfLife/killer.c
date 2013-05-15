//
//  parents_killer.c
//  GameOfLife
//
//  Created by JIANNAN GUO on 12.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "cell_struct.h"

void kill_cell(cell_list *alive_cell_l) {
    free(alive_cell_l->ptr);
    free(alive_cell_l);
}

void kill_msg(unsigned char *msg){
    int i;
    for (i=0; i<SIZE_OF_FIELD; i++) {
        msg[i] = 0;
    }
}
