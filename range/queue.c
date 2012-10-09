/* queue.c */

#include "Arduino.h"
#include "queue.h"

int queue_enqueue(queue *q, queue_event *e) {
  if (q->full) return 1;

  uint8_t sreg_save = SREG;
  cli();

  q->e[q->in++] = *e;
  if (q->in == QUEUE_SIZE) q->in = 0;
  if (q->in == q->out) q->full = 1;

  SREG = sreg_save;

  return 0;
}

int queue_deque(queue *q, queue_event *e) {
  int rc = 0;
  uint8_t sreg_save = SREG;
  cli();

  if (q->in != q->out || q->full) {
    *e = q->e[q->out++];
    if (q->out == QUEUE_SIZE) q->out = 0;
    q->full = 0;
    rc = 1;
  }

  SREG = sreg_save;

  return rc;
}

void queue_empty(queue *q) {
  uint8_t sreg_save = SREG;
  cli();
  q->in = q->out = q->full = 0;
  SREG = sreg_save;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
