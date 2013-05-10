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
#define FIRST_PROCESS 1
#define LAST_PROCESS NUM_OF_PROCESS

#define COUNT_LENGTH SIZE_OF_FIELD+4
#define COUNT_HEIGHT SIZE_OF_UNIT+4

#define SEQ_OF_PROCESS 1

struct cell {
    int h;
    int v;
};

struct cell_list {
    int len;
    struct cell *ptr;
};

extern int COUNT_FIELD[COUNT_HEIGHT][COUNT_LENGTH];

#endif
