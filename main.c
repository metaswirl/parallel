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
#include "cell_struct.h"
#include "v_extension.h"

void printcell(const struct cell_list *print_cell_l);

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
    
    struct cell_list wholefield_l = {
        10,
        fullfield
    };
    
    struct cell_list *local;
    local = calloc(512, sizeof(local));
    
    local = divide_field(SEQ_OF_PROCESS, &wholefield_l);
    
    printcell(local);
    
    v_extend(local);
    
    printcell(local);
    
    return 0;
}

void printcell(const struct cell_list *print_cell_l) {
    int i, len;
    struct cell *print_cell;
    len = print_cell_l->len;
    print_cell = print_cell_l->ptr;
    printf("print content of structure\n");
    for (i=0; i<len; i++) {
        printf("%d : (%d, %d)\n", i, print_cell[i].x, print_cell[i].y);
    }
    printf("----------------------\n");
}