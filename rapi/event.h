#ifndef __EVENT_H
#define __EVENT_H

#include <stdint.h>

typedef struct event {
  struct event *next;
  uint16_t addr;
  uint8_t ov, nv;
} event_t;

event_t *event_get(  );
event_t *event_make( uint16_t addr, uint8_t ov, uint8_t nv );
void event_release( event_t * event );

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c 
 */
