/* synth.h */

#ifndef __SYNTH_H
#define __SYNTH_H

#include <stdint.h>

typedef struct {
  uint32_t rate;
  uint32_t phase;
  uint32_t phase_increment;
} synth;

void synth_set_rate(synth *sy, uint32_t rate);
void synth_set_frequency(synth *sy, uint32_t hz);

int16_t synth_sin(synth *sy);

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
