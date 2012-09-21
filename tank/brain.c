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

static mavg prox_avg[1];

static MAVG_DTYPE cturn, cdrive;
static MAVG_DTYPE lprox;

void
br_init( void ) {
  mavg_init_multi( prox_avg, countof( prox_avg ) );
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
/*  MAVG_DTYPE base_prox = mavg_update( prox_avg, prox );*/
/*  MAVG_DTYPE dprox = prox - base_prox;*/
  MAVG_DTYPE dprox = prox - lprox;
  lprox = prox;

  if ( dprox > 0 ) {
    MAVG_DTYPE turn = lprox - rprox;
    MAVG_DTYPE dir = sgn( turn );
    MAVG_DTYPE diff = abs( turn );

    cturn = dprox * ( 30 - min( diff, 30 ) ) * dir;
  }

  cturn -= sgn( cturn );

  *ldrive = clip( -255, cdrive - cturn, 255 );
  *rdrive = clip( -255, cdrive + cturn, 255 );
}

/* vim:ts=2:sw=2:sts=2:et:ft=c 
 */
