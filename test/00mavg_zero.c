/* mavg_test.c */

#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../mavg.h"

#include "tap.h"

#define RATE       48000
#define WAVELENGTH (RATE/20) /* 20Hz sampled at 48kHz */
#define DURATION   10        /* 10s */
#define DEPTH      4
#define RIPPLE     7

int main( void ) {
  unsigned i, bad;
  double a;
  mavg *m = mavg_new( DEPTH );
  double step = 2 * M_PI / WAVELENGTH;

  plan( 1 );

  for ( i = 0, a = 0, bad = 0; i < DURATION * RATE; i++, a += step ) {
    double sample = sin( a ) * 127;
    MAVG_DTYPE dd = mavg_update( m, ( MAVG_DTYPE ) round( sample ) );
    if ( abs( dd ) > RIPPLE ) bad++;
  }

  if ( !is( bad, 0, "No samples out of range %d..%d", -RIPPLE,  RIPPLE ) )
    diag( "%u samples out of range", bad );

  mavg_free( m );
  return 0;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
