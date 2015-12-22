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
  cog.outl("#define MSG_%s_ID %d" % (msg["name"].upper(), msg["id"]))
  cog.outl("typedef struct {")
  cog.outl("\tuint8_t msg_id;")

  if "fields" in msg:
    for f in msg["fields"]:
      cog.outl("\t%s %s;" % (f["type"], f["name"]))

  cog.outl("} %s_t;" % msg["name"])
  cog.outl()

]]]*/
//[[[end]]]
