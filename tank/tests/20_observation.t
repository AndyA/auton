/* observation.t */

#include "tap.h"
#include "observation.h"

Observation<int16_t, 50> obs;

static void fill( int16_t d, int n = 50 ) {
  int i;
  for ( i = 0; i < n; i++ )
    obs.push( d );
}

static void min_max( ) {
  fill( -1000 );
  is( obs.getMin(), -1000, "min = -1000" );
  is( obs.getMax(), -1000, "max = -1000" );
  obs.push( 1000 );
  is( obs.getMin(), -1000, "min = -1000" );
  is( obs.getMax(), 1000, "max = 1000" );
}

static void mean_rms() {
  fill( -123 );
  fill( 123, 25 );
  is( obs.getMean(), 0, "mean = 0" );
  is( obs.getRMS(), 123, "rms = 123" );
  fill( 500 );
  is( obs.getRMS(), 0, "rms = 0" );
  fill( 700, 25 );
  is( obs.getRMS(), 100, "rms = 100" );
}

int
main( void ) {
  plan( 8 );
  min_max();
  mean_rms();
  return 0;
}

/* vim:ts=2:sw=2:sts=2:et:ft=cpp
 */
