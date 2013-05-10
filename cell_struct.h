//
//  cell_struct.h
//  GameOfLife
//
//  Created by JIANNAN GUO on 10.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//

#ifndef GameOfLife_cell_struct_h
#define GameOfLife_cell_struct_h

#define SIZE_OF_FIELD 64
#define NUM_OF_PROCESS 8
#define SIZE_OF_UNIT (SIZE_OF_FIELD / NUM_OF_PROCESS)
#define SEQ_OF_PROCESS 1
#define FIRST_PROCESS 1
#define LAST_PROCESS NUM_OF_PROCESS

struct cell {
    int h;
    int v;
};

struct cell_list {
    int len;
    struct cell *ptr;
};

extern int **COUNT_FIELD;

#endif
