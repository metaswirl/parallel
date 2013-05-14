//
//  message.h
//  GameOfLife
//
//  Created by JIANNAN GUO on 11.05.13.
//  Copyright (c) 2013 Jiannan Guo. All rights reserved.
//

#ifndef GameOfLife_message_h
#define GameOfLife_message_h

void msg_extract(char *direction, const cell_list *alive_list_l, unsigned char *msg);
cell_list *msg_handle(unsigned char *msg_from_top, unsigned char *msg_from_bottom, cell_list *alive_cell_l);

#endif
