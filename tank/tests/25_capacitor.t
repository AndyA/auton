/* capacitor.t */

#include "tap.h"
#include "instinct.h"

Capacitor cap;

static void test_capacitor() {
  int16_t v;
  v = 123;

  cap.discharge( &v, 255 );
  is( v, 0, "empty cap, zero" );

  cap.charge( 100 );
  cap.discharge( &v, 10 );
  is( v, 10, "got 10" );

  cap.discharge( &v, 80 );
  is( v, 80, "got 80" );

  cap.discharge( &v, 80 );
  is( v, 10, "got 10" );

  cap.charge( 10 );
  cap.discharge( &v, 5 );
  is( v, 5, "got 5" );

  cap.charge( -10 );
  cap.discharge( &v, 5 );
  is( v, -5, "got -5" );
}

int main( void ) {
  plan( 6 );
  test_capacitor();
  return 0;
}

// vim:ts=2:sw=2:sts=2:et:ft=cpp

