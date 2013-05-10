#include <stdio.h>
#include <stdlib.h>
#include "localize.h"
#include "cell_struct.h"

struct cell_list * divide_field(int seq_of_process, const struct cell_list *full_alive_l) {
    int i, length_in, length_out=0;
    struct cell *alive_list;
    int left_border, right_border;
    length_in = full_alive_l->len;
    alive_list = full_alive_l->ptr;
    struct cell *localized_list;
    struct cell_list *localized_list_t;
    localized_list_t = calloc(1, sizeof(localized_list_t));
    localized_list = calloc(512, sizeof(localized_list));
    // determine the edges on both sides
    if (seq_of_process < NUM_OF_PROCESS && seq_of_process > 1) {
        // not edge, process in the center area.
        left_border = SIZE_OF_UNIT * (seq_of_process - 1);
        right_border = SIZE_OF_UNIT * (seq_of_process);
        for(i=0; i<length_in; i++) {
            if(alive_list[i].x >= left_border && alive_list[i].x <= right_border) {
                localized_list[length_out].x = alive_list[i].x;
                localized_list[length_out].y = alive_list[i].y;
                length_out++;
            }
        }
    } else if (seq_of_process == 1) {
        // left border process.
        right_border=SIZE_OF_UNIT;
        for(i=0; i<length_in; i++) {
            if((alive_list[i].x >= 0 && alive_list[i].x <= right_border) || (alive_list[i].x == SIZE_OF_FIELD - 1)) {
                localized_list[length_out].x = alive_list[i].x;
                localized_list[length_out].y = alive_list[i].y;
                length_out++;
            }
        }
    } else if (seq_of_process == NUM_OF_PROCESS) {
        // right border process.
        left_border=SIZE_OF_FIELD-NUM_OF_PROCESS;
        for(i=0; i<length_in; i++) {
            if((alive_list[i].x >= left_border && alive_list[i].x <= SIZE_OF_FIELD - 1) || (alive_list[i].x == 0)) {
                localized_list[length_out].x = alive_list[i].x;
                localized_list[length_out].y = alive_list[i].y;
                length_out++;
            }
        }
    }
    
    localized_list_t->len = length_out;
    localized_list_t->ptr = localized_list;
    
    return localized_list_t;
}



