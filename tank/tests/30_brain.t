/* brain.t */

#include "tap.h"
#include "brain.h"

static Brain brain;
static ExploreInstinct explore;
static SteerInstinct steer;

static void init_brain() {
  brain.addInstinct( &explore );
  brain.addInstinct( &steer );
}

static void test_brain() {
  int16_t turn = 10, drive = 0;
  brain.update( 0, 0, &turn, &drive );
  is( turn, 0, "turn zero" );
  is( drive, 255, "full speed ahead" );
}

int main( void ) {
  init_brain();
  plan( 2 );
  test_brain();
  return 0;
}

// vim:ts=2:sw=2:sts=2:et:ft=cpp
