//
//  main.c
//  dividefield
//
//  Created by JIANNAN GUO on 09.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "localize.h"

int main()
{
    struct cell fullfield[10] = {
        {0,0},
        {5,5},
        {10,10},
        {20,20},
        {30,30},
        {40,40},
        {50,50},
        {63,63},
        {0,63},
        {63,0}
    };
    
    struct cell_list wholefield = {
        10,
        fullfield
    };
    
    int len, i;
    struct cell *localized_list;
    
    struct cell_list *local;
    local = calloc(1, sizeof(local));
    
    local = divide_field(8, &wholefield);
    len = local->len;
    localized_list = local->ptr;
    
    for (i=0; i<len; i++) {
        printf("(%d, %d)\n", localized_list[i].x, localized_list[i].y);
    }
    
    return 0;
}