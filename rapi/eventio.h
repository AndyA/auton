/* eventio.h */

#ifndef __EVENTIO_H
#define __EVENTIO_H

#include "eventdef.h"

#ifdef __cplusplus
extern "C" {
#endif

  ssize_t evio_read(int fd, struct event *ev);

#ifdef __cplusplus
}
#endif

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
