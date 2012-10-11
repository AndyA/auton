/* synth.c */

#include <stdio.h>
#include <stdint.h>
#include "itrig.h"
#include "synth.h"

void synth_set_rate(synth *sy, uint32_t rate) {
  sy->rate = rate;
}

void synth_set_frequency(synth *sy, uint32_t hz) {
  sy->phase_increment = hz * ITRIG_CIRCLE / sy->rate;
}

void synth_set_amplitude(synth *sy, uint16_t amp) {
  sy->amplitude = amp;
}

int16_t synth_sin(synth *sy) {
  int32_t s = itrig_sin(sy->phase);
  sy->phase += sy->phase_increment;
  while (sy->phase > ITRIG_CIRCLE)
    sy->phase -= ITRIG_CIRCLE;
  return ((s >> 16) * sy->amplitude) >> 16;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
