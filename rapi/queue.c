
#include "queue.h"
#include <pthread.h>

void
queue_enqueue( queue_t * queue, event_t * event ) {
  pthread_mutex_lock( &( queue->mutex ) );
  if ( queue->tail ) {
    queue->tail->next = event;
  }
  else {
    queue->head = event;
  }
  queue->tail = event;
  event->next = NULL;
  pthread_mutex_unlock( &( queue->mutex ) );
  pthread_cond_broadcast( &( queue->cond_empty ) );
}

event_t *
queue_dequeue( queue_t * queue ) {
  pthread_mutex_lock( &( queue->mutex ) );
  while ( !queue->head ) {
    pthread_cond_wait( &( queue->cond_empty ), &( queue->mutex ) );
  }
  event_t *event = queue->head;
  if ( !( queue->head = event->next ) ) {
    queue->tail = NULL;
  }
  pthread_mutex_unlock( &( queue->mutex ) );
  return event;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c 
 */
