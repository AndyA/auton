/* brain.c */

#include "Arduino.h"

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

#define FAR     60
#define NEAR    210
#define NEUTRAL 186

static mavg lprox_avg[1], rprox_avg[1], turn_avg[2];

static MAVG_DTYPE cturn, cdrive;
static MAVG_DTYPE pprox;

void
br_init( void ) {
  mavg_init_multi( lprox_avg, countof( lprox_avg ) );
  mavg_init_multi( rprox_avg, countof( rprox_avg ) );
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
  MAVG_DTYPE slprox = mavg_update( lprox_avg, lprox );
  MAVG_DTYPE srprox = mavg_update( rprox_avg, rprox );
  MAVG_DTYPE prox = max( slprox, srprox );
  MAVG_DTYPE dprox = prox - pprox;
  MAVG_DTYPE turn = slprox - srprox;
  MAVG_DTYPE sturn = mavg_update( turn_avg, turn );
  MAVG_DTYPE dturn = turn - sturn;
  pprox = prox;

  if ( dprox > 0 ) {
    MAVG_DTYPE dir = sgn( dturn );
    MAVG_DTYPE diff = abs( dturn );

    MAVG_DTYPE adj = dprox * dprox * ( 100 - min( diff, 99 ) ) * dir / 100;
    cturn = ( cturn + adj ) / 2;

/*    Serial.print( "dprox=" );*/
/*    Serial.print( dprox );*/
/*    Serial.print( ", dturn=" );*/
/*    Serial.print( dturn );*/
/*    Serial.print( ", dir=" );*/
/*    Serial.print( dir );*/
/*    Serial.print( ", diff=" );*/
/*    Serial.print( diff );*/
/*    Serial.print( ", adj=" );*/
/*    Serial.print( adj );*/
/*    Serial.print( ", cturn=" );*/
/*    Serial.println( cturn );*/
  }

  if ( prox > NEAR ) {
    cdrive = -50;
    cturn = 0;
  }
  else if ( prox > NEUTRAL ) {
    cdrive /= 2;
    cturn = sgn( dturn ) * 30;
  }
  else {
    cdrive = min( 255, cdrive + 3 );
  }

  cturn -= sgn( cturn );
/*  if ( cturn )*/
/*    Serial.println( cturn );*/

  *ldrive = clip( -255, cdrive - cturn, 255 );
  *rdrive = clip( -255, cdrive + cturn, 255 );
}

/* vim:ts=2:sw=2:sts=2:et:ft=c 
 */
