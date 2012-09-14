#include "event.h"
#include "util.h"
#include <stdlib.h>
#include <pthread.h>

static event_t *free_list = NULL;
pthread_mutex_t free_mtx = PTHREAD_MUTEX_INITIALIZER;

event_t *
event_get(  ) {
  event_t *event;
  pthread_mutex_lock( &free_mtx );
  if ( free_list ) {
    event = free_list;
    free_list = event->next;
    event = free_list;
    return event;
  }
  else {
    event = alloc( sizeof( event_t ) );
  }
  pthread_mutex_unlock( &free_mtx );
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
  pthread_mutex_lock( &free_mtx );
  event->next = free_list;
  free_list = event;
  pthread_mutex_unlock( &free_mtx );
}

/* vim:ts=2:sw=2:sts=2:et:ft=c 
 */
