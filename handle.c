#include <stdlib.h>
#include "parse.h"

/*[[[cog
import cog
import json
from gen.helpers import *

msg_file = load_msg_file()

reqs = generate_req_msgs(msg_file["messages"])
all_msgs = msg_file["messages"] + reqs

last_msg_id = find_max_msg_id(all_msgs)

# The output is generated from the lines below

insert_header()
cog.outl("#define LAST_MSG_ID %s" % last_msg_id)

]]]*/
//[[[end]]]

void (*handlers[LAST_MSG_ID + 1])(frame_t*) = {NULL};
void (*default_handler)(frame_t*) = NULL;

void init_handlers() {
   for(int i = 0; i < LAST_MSG_ID + 1; i++) {
     handlers[i] = NULL;
   }
}

void set_handler(uint8_t msg_id, void (*handler)(frame_t*)) {
  if(msg_id <= LAST_MSG_ID)
    handlers[msg_id] = handler;
}

void set_default_handler(void (*handler)(frame_t*)) {
  default_handler = handler;
}

void handle_frame(frame_t *frm) {
  if((frm->id > LAST_MSG_ID))
    return;

  if(handlers[frm->id] != NULL)
    handlers[frm->id](frm);
  else
    if(default_handler != NULL)
      default_handler(frm);
}
