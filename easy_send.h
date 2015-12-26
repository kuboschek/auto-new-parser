#ifndef _EASY_SEND_H_
#define _EASY_SEND_H_

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

for msg in all_msgs:
  f_str = ""

  if "fields" in msg:
    for f in msg["fields"]:
      f_str += "%s %s, " % (f["type"], f["name"])

  sign = "void send_%s_msg(%svoid (*send_byte)(uint8_t));" % (msg["name"].lower(), f_str)

  cog.outl(sign)
  cog.outl()

]]]*/
//[[[end]]]

#endif
