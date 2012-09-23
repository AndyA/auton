/* sim.cpp */

#include <stdio.h>
#include <stdint.h>
#include "debug.h"
#include "brain.h"
#include "common.h"

static Brain brain;
static ExploreInstinct explore;
static SteerInstinct steer;

static void init_brain() {
  brain.addInstinct( &explore );
  brain.addInstinct( &steer );
}

int main( void ) {
  int lprox, rprox;
  int16_t turn = 0, drive = 0;

  db_set_level( LL_DEBUG );

  init_brain();
  while ( EOF != fscanf( stdin, "%d,%d\n", &lprox, &rprox ) ) {
    brain.update( lprox, rprox, &turn, &drive );
    INFO( ( "lprox=%d, rprox=%d, turn=%d, drive=%d",
            lprox, rprox, ( int ) turn, ( int ) drive ) );
    db_bump_time( 40 );
  }

  return 0;
}

// vim:ts=2:sw=2:sts=2:et:ft=cpp
