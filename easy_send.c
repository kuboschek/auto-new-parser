#include "parse.h"
#include "msg_defs.h"

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

  msg_id_string = "MSG_%s_ID" % msg["name"].upper()

  sign = "void send_%s_msg(%svoid (*send_byte)(uint8_t)) {" % (msg["name"].lower(), f_str)

  cog.outl(sign)
  cog.outl("\t%s_t things;" % msg['name'].lower())
  cog.outl("\tframe_t rsp;")
  cog.outl()

  cog.outl("\trsp.data = (char*) &things;")
  cog.outl("\trsp.data_len = sizeof(things);")
  cog.outl()

  cog.outl("\tthings.msg_id = %s;" % msg_id_string)

  if "fields" in msg:
    for f in msg["fields"]:
      cog.outl("\tthings.%s = %s;" % (f["name"], f["name"]))

  cog.outl()
  cog.outl("\tsend_msg(&rsp, send_byte);")

  cog.outl("}")
  cog.outl()

]]]*/
//[[[end]]]
