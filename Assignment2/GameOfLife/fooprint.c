//
//  fooprint.c
//  GameOfLife
//
//  Created by JIANNAN GUO on 10.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//

#include <stdio.h>
#include "cell_struct.h"

void print_count_matrix(void) {
    printf("the count matrix\n");
    int j, k;
    for (j=COUNT_HEIGHT-1; j>=0; j--) {
        for (k=0; k<COUNT_LENGTH; k++) {
            printf("%d ", COUNT_FIELD[j][k]);
        }
        printf("\n");
    }
    printf("--------------------------------------\n");
}

void printcell(const struct cell_list *print_cell_l) {
    int i, len;
    struct cell *print_cell;
    len = print_cell_l->len;
    print_cell = print_cell_l->ptr;
    printf("print alive cell list\n");
    for (i=0; i<len; i++) {
        printf("%d : (%d, %d)\n", i, print_cell[i].h, print_cell[i].v);
    }
    printf("-----------------------------\n");
}

void printcell_vividly(const struct cell_list *print_cell_t) {
    int show[COUNT_HEIGHT][COUNT_LENGTH] = {0};
    int i, j, k, len = print_cell_t->len;
    struct cell *print_cell;
    print_cell = print_cell_t->ptr;
    printf("print alive cell vividly\n");
    for (i=0; i<len; i++) {
        show[print_cell[i].v][print_cell[i].h] = 1;
    }
    for (j=0; j<COUNT_HEIGHT; j++) {
        for (k=0; k<COUNT_LENGTH; k++) {
            printf("%d ", show[j][k]);
        }
        printf("\n");
    }
    printf("--------------------------------------\n");
}