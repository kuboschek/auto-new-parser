/*
* parse.c
*
*  Created on: 12.08.2015
*      Author: leonhard
*/


#include "uart.h"
#include "pfdb.h"

#include "parse.h"
#include "stdio.h"

#include <util/crc16.h>
#include <stdlib.h>

#define STX_VAL 0xEE
#define ESC_VAL 0x30
#define DEBUG

enum parse_state the_state = IDLE;
enum parse_state prev_state = IDLE;
uint8_t data_pos = 0;

void print_byte(uint8_t byte) {
  char str[3];

  sprintf(str, "%X", byte);
  uart1_puts(str);
}

int parse_byte(frame_t* frm, uint8_t byte) {
  if(frm == NULL)
    return -1;

  if(the_state == AFTER_ESC) {
    // Restore previous state
    the_state = prev_state;
  } else if(the_state != STX && the_state != IDLE) {
    if(byte == STX_VAL) {
      // Dump current message, a new one is arriving
      prev_state = the_state = IDLE;
      if(frm->data != NULL) {
        free(frm->data);
      }
    }

    if(byte == ESC_VAL) {
      prev_state = the_state;
      the_state = AFTER_ESC;
      return 0;
    }
  }

  switch(the_state) {
    case IDLE:
      if(byte == STX_VAL) {
        the_state = STX;
      }
      break;

    case STX:
      if(byte == STX_VAL) {
        the_state = LEN;
      } else {
        the_state = IDLE;
      }
      break;

    case LEN:
      frm->data_len = byte;
      data_pos = 0;

      frm->data = (char*) malloc(frm->data_len);

      if(frm->data == NULL) {
        the_state = IDLE;
        return -1;
      }

      the_state = DATA;
      break;

    case DATA:
      /*
      uart1_puts("datapos: ");
      print_byte(data_pos);
      uart1_puts(" inbyte: ");
      print_byte(byte);
      uart1_puts("\n");
      */

      frm->data[data_pos] = byte;
      data_pos++;

      if(data_pos == frm->data_len) {
        the_state = CRCH;
      }
      break;

    case CRCH:
      frm->crc = byte << 8;

      frm->id = frm->data[0];

      the_state = CRCL;
      break;

    case CRCL:
      frm->crc |= (byte & 0xFF);
      uint16_t calc_crc = 0xFFFF;

      for(int i = 0; i < frm->data_len; i++) {
        calc_crc = _crc16_update(calc_crc, frm->data[i]);
      }

      //dbg_info("Msg CRC is: %d, Calc CRC is: %d", frm->crc, calc_crc);

      the_state = IDLE;

      if(calc_crc != frm->crc) {
        uart1_puts("Incorrect CRC.\n");
      }

      if(calc_crc == frm->crc)
        return 1;
      else
        return -1;

      break;
  }

  return 0;
}

void escape(uint8_t byte, void (*send_byte)(uint8_t)) {
  if(byte == STX_VAL || byte == ESC_VAL)
    send_byte(ESC_VAL);

  send_byte(byte);
}

int send_msg(frame_t* frm, void (*send_byte)(uint8_t)) {
  if(frm == NULL)
    return -1;

  //Update CRC
  uint16_t calc_crc = 0xFFFF;


  for(int i=0; i < frm->data_len; i++) {
    calc_crc = _crc16_update(calc_crc, frm->data[i]);
  }

  frm->crc = calc_crc;

  //STX
  send_byte(STX_VAL);
  send_byte(STX_VAL);

  //LEN
  escape(frm->data_len, send_byte);

  //DATA
  for(int i = 0; i < frm->data_len; i++)
    escape(frm->data[i], send_byte);

  //CRC
  escape((char) (frm->crc >> 8), send_byte);
  escape((char) (frm->crc & 0xFF), send_byte);


  return 1;
}
