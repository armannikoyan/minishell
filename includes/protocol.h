#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <sys/ioctl.h>

typedef enum {
  MSG_DATA = 0,
  MSG_WINCH = 1,
  MSG_DETACH = 2,
  MSG_CREATE_WIN = 3,
  MSG_SWITCH_WIN = 4,
  MSG_REDRAW_UI = 5
} e_msg_type;

typedef struct __attribute__((packed)) {
  uint8_t type;
  uint32_t len;
} t_msg_header;

#endif