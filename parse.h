/*
* parse.h
*
*  Created on: 12.08.2015
*      Author: leonhard
*/

#ifndef PARSE_H_
#define PARSE_H_

#include <stdint.h>

enum parse_state {
  IDLE,   // First STX -> STX state
  STX,    // Second STX -> LEN state
  LEN,    // Length byte -> DATA state
  DATA,   // More data -> DATA state, last data -> CRCH state
  CRCH,   // CRCH byte -> CRCL state
  CRCL    // CRCL byte -> check CRC, IDLE state
};

typedef struct {
  uint8_t id;
  uint8_t data_len;
  char* data;

  uint16_t crc;
} frame_t;

int parse_byte(frame_t* frm, uint8_t byte);
int send_msg(frame_t* frm, void (*send_byte)(uint8_t));

#endif /* PARSE_H_ */
