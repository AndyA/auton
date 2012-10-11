/* itrig.h */

#ifndef __ITRIG_H
#define __ITRIG_H

#include <stdint.h>

#define ITRIG_CIRCLE 16384

/* angle 0..3600 - degrees * 10 */
int32_t itrig_sin(int32_t angle);
int32_t itrig_cos(int32_t angle);

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
