/* brain.c */

#include "brain.h"
#include "mavg.h"

#include <stdlib.h>

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef clip
#define clip(a, b, c) max(a, min(b, c))
#endif
#ifndef countof
#define countof(a) (sizeof(a) / sizeof((a)[0]))
#endif
#ifndef sgn
#define sgn(x) ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)
#endif

static mavg prox_avg[2];
static mavg turn_avg[2];
static mavg drive_avg[2];

void
br_init( void ) {
  mavg_init_multi( prox_avg, countof( prox_avg ) );
  mavg_init_multi( turn_avg, countof( turn_avg ) );
  mavg_init_multi( drive_avg, countof( drive_avg ) );
}

static MAVG_DTYPE
br__delta( mavg * m, MAVG_DTYPE d, uint8_t tap ) {
  MAVG_DTYPE base = mavg_update( m, d );
  MAVG_DTYPE hot = mavg_peek( &m[tap] );
  return hot - base;
}

void
br_update( uint16_t lprox, uint16_t rprox, int16_t * ldrive,
           int16_t * rdrive ) {
  MAVG_DTYPE prox = max( lprox, rprox );
  MAVG_DTYPE base_prox = mavg_update( prox_avg, prox );
  MAVG_DTYPE hot_prox = mavg_peek( &prox_avg[1] );
  MAVG_DTYPE dprox = hot_prox - base_prox;
  MAVG_DTYPE nturn = 0;
  MAVG_DTYPE ndrive = 0;

  if ( dprox > 0 ) {
    MAVG_DTYPE turn = lprox - rprox;
    MAVG_DTYPE dir = sgn( turn );
    MAVG_DTYPE diff = abs( turn );
    nturn = dprox * ( 20 - min( diff, 20 ) ) * dir * 4;
  }

  {
    MAVG_DTYPE turn = mavg_update( turn_avg, nturn );
    MAVG_DTYPE drive = max( 0, mavg_update( drive_avg, ndrive ) );

    *ldrive = clip( -255, drive - turn, 255 );
    *rdrive = clip( -255, drive + turn, 255 );
  }
}

/* vim:ts=2:sw=2:sts=2:et:ft=c 
 */
