//
//  localize.h
//  dividefield
//
//  Created by JIANNAN GUO on 09.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//

#ifndef dividefield_localize_h
#define dividefield_localize_h

struct cell {
    int x;
    int y;
};

struct cell_list {
    int len;
    struct cell *ptr;
};

struct cell_list * divide_field(int seq_of_process, const struct cell_list *full_alive_t);

#endif
