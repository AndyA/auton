/* event.h */

#ifndef __EVENT_H
#define __EVENT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
  QT_RANGE,
  QT_INDEX
};

struct event {
  uint32_t ts;
  uint8_t type;
  union {
    struct {
      uint16_t r;
      uint8_t c;
    } rf;
    struct {
      /* empty */
    } idx;
  } d;
};

#ifdef __cplusplus
}
#endif

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
