#include "event.h"
#include "util.h"
#include <stdlib.h>

static event_t *free_list = NULL;

event_t *
event_get(  ) {
  event_t *event;
  if ( free_list ) {
    event = free_list;
    free_list = event->next;
    return event;
  }
  return alloc( sizeof( event_t ) );
}

event_t *
event_make( uint16_t addr, uint8_t ov, uint8_t nv ) {
  event_t *event = event_get(  );
  event->next = NULL;
  event->addr = addr;
  event->ov = ov;
  event->nv = nv;
  return event;
}

void
event_release( event_t * event ) {
  event->next = free_list;
  free_list = event;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c 
 */
