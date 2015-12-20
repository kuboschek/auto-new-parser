

void handle_frame(frame_t *frm) {
  switch(frm->id) {
    case MSG_MODUS_ID:
      msg_modus_t *msg = (msg_modus_t*) frm->data;

      handle_modus_msg(msg);

      break;
    // TODO Generate cases for each message
  }
}

void handle_modus_msg(msg_modus_t* msg) {

}
