#ifndef _QUEUE_H
#define _QUEUE_H

#include <pthread.h>
#include <stdint.h>
#include "event.h"

#define QUEUE_INITIALIZER { \
  NULL, NULL, \
  PTHREAD_MUTEX_INITIALIZER, \
  PTHREAD_COND_INITIALIZER \
}

typedef struct queue {
  event_t *head;
  event_t *tail;
  pthread_mutex_t mutex;
  pthread_cond_t cond_empty;
} queue_t;

void queue_enqueue( queue_t * queue, event_t * event );
event_t *queue_dequeue( queue_t * queue );

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c 
 */
