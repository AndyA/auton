/* observation.t */

#include <stdio.h>
#include "tap.h"
#include "observation.h"

Observation<int16_t, 50> obs;

static void fill( int16_t d ) {
  int i;
  for ( i = 0; i < 50; i++ )
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

int
main( void ) {
  plan( 4 );
  min_max();
  return 0;
}

/* vim:ts=2:sw=2:sts=2:et:ft=cpp
 */
