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
#include "h_extension.h"
#include "normalize.h"

void printcell(const struct cell_list *print_cell_l);

int main()
{
    int COUNT_FIELD[SIZE_OF_UNIT + 4][SIZE_OF_FIELD + 4];
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
    
    normalize(SEQ_OF_PROCESS, local);
    
    printcell(local);
    
    h_extend(local);
    
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
        printf("%d : (%d, %d)\n", i, print_cell[i].h, print_cell[i].v);
    }
    printf("-----------------------------\n");
}