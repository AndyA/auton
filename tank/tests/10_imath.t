/* imath.t */

#include <math.h>
#include <stdlib.h>
#include "tap.h"
#include "imath.h"

static void test_sqrt() {
  uint32_t i;
  for ( i = 0; i < 1000; i++ ) {
    uint32_t in = rand();
    uint32_t want = ( uint32_t ) sqrt( in );
    uint32_t got = isqrt( in );
    is( got, want, "sqrt(%lu) = %lu", in, got );
  }
}

int main( void ) {
  plan( 1000 );
  test_sqrt();
  return 0;
}

/* vim:ts=2:sw=2:sts=2:et:ft=cpp
 */
