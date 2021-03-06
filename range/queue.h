/* queue.h */

#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdint.h>
#include "eventdef.h"

#ifndef QUEUE_SIZE
#define QUEUE_SIZE 10
#endif

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct event queue_event;

  typedef struct {
    uint8_t in, out, full;
    queue_event e[QUEUE_SIZE];
  } queue;

  int queue_enqueue(queue *q, queue_event *e);
  int queue_deque(queue *q, queue_event *e);
  void queue_empty(queue *q);

#ifdef __cplusplus
}
#endif

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
