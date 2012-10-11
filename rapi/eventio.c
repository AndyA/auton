/* eventio.c */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "util.h"
#include "eventio.h"

struct bp {
  unsigned char *p;
  int l;
};

static int evio__g8(struct bp *bp, uint8_t *v) {
  if (bp->l < 1) return -1;
  bp->l--;
  *v = *(bp->p++);
  return 0;
}

static int evio__g16(struct bp *bp, uint16_t *v) {
  if (bp->l < 2) return -1;
  bp->l -= 2;
  *v = *(bp->p++);
  *v |= *(bp->p++) << 8;
  return 0;
}

static int evio__g32(struct bp *bp, uint32_t *v) {
  if (bp->l < 4) return -1;
  bp->l -= 4;
  *v = *(bp->p++);
  *v |= *(bp->p++) << 8;
  *v |= *(bp->p++) << 16;
  *v |= *(bp->p++) << 24;
  return 0;
}

static int evio__decode(struct event *ev, unsigned char *buf, int len) {
  struct bp p = { buf, len };
  if (evio__g32(&p, &ev->ts) || evio__g8(&p, &ev->type)) return -1;
  switch (ev->type) {
  case QT_RANGE:
    if (evio__g16(&p, &ev->d.rf.r) || evio__g8(&p, &ev->d.rf.c)) return -1;
    break;
  case QT_INDEX:
    /* empty */
    break;
  default:
    return 0;
  }
  return 0;
}

ssize_t evio_read(int fd, struct event *ev) {
  ssize_t rc;
  int len, elen;
  unsigned char *bp, buf[sizeof(struct event)];

#define RB(b) \
  do { if (rc = read(fd, buf, (b)), rc <= 0) goto done; } while (0)

  // Loop forever for sync character
  do {
    RB(1);
  }
  while (buf[0] != 'E');

  RB(1);

  if (buf[0] > sizeof(buf)) {
    rc = -1000;
    goto done;
  }

  elen = len = buf[0];
  bp = buf;

  while (len != 0) {
    if (rc = read(fd, bp, len), rc <= 0) goto done;
    bp += rc;
    len -= rc;
  }

#undef RB

  rc = evio__decode(ev, buf, elen);
  if (rc == 0) rc = 1;

done:
  return rc;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
