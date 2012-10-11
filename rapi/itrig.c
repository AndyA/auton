/* itrig.c */

#include "itrig.h"

#define ITRIG_HC (ITRIG_CIRCLE/2)
#define ITRIG_QC (ITRIG_CIRCLE/4)

extern int32_t itrig_sin_table[];

/* angle 0..ITRIG_CIRCLE - degrees * 10 */
int32_t itrig_sin(int32_t angle) {
  if (angle < 0) return -itrig_sin(-angle);
  if (angle <= ITRIG_QC) return itrig_sin_table[angle];
  if (angle <= ITRIG_HC) return itrig_sin(ITRIG_HC - angle);
  if (angle <= ITRIG_CIRCLE) return -itrig_sin(angle - ITRIG_HC);
  return itrig_sin(angle % ITRIG_CIRCLE);
}

int32_t itrig_cos(int32_t angle) {
  return itrig_sin(angle + ITRIG_QC);
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
