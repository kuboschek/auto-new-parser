

#ifndef _HANDLE_H_
#define _HANDLE_H_

#include "parse.h"

void init_handlers();

void set_handler(uint8_t msg_id, void (*handler)(frame_t*));

void set_default_handler(void (*handler)(frame_t*));

void handle_frame(frame_t *frm);

#endif
