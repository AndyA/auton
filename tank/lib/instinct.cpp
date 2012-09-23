#include "instinct.h"
#include "imath.h"
#include "common.h"

#define FAR     60
#define NEAR    200
#define NEUTRAL 150

void Capacitor::charge( int32_t e ) {
  if ( sgn( energy ) == sgn( e ) ) energy += e;
  else energy = e;
}

void Capacitor::discharge( int16_t *sink, int16_t max ) {
  int32_t e = energy;
  if ( iabs( e ) > max ) e = e < 0 ? -max : max;
  *sink = e;
  energy -= e;
}

void Context::update( int16_t l, int16_t r, int16_t t, int16_t d ) {
  lprox.push( l );
  rprox.push( r );
  turn.push( t );
  drive.push( d );
}

int16_t Context::getProx() {
  return max( lprox.peek( -1 ), rprox.peek( -1 ) );
}

uint8_t ExploreInstinct::consider( Context *ctx ) {
  return 1;
}

void ExploreInstinct::apply( Context *ctx, Insight *res ) {
  res->drive = 255; // hit it!
}

uint8_t SteerInstinct::consider( Context *ctx ) {
  return 1;
}

void SteerInstinct::apply( Context *ctx, Insight *res ) {
  int16_t dl = ctx->lprox.getDelta();
  int16_t dr = ctx->rprox.getDelta();
  int16_t delta = max( max( dl, dr ), 0 );
  int16_t diff = dl - dr;

  if ( ctx->getProx() > NEUTRAL && delta > 3 )
    cap.charge( sgn( diff ) * delta * 2 );

  cap.discharge( &res->turn, 255 );
}

uint8_t StopInstinct::consider( Context *ctx ) {
  if ( ctx->getProx() > NEAR ) return 2;
  return 0;
}

void StopInstinct::apply( Context *ctx, Insight *res ) {
  DEBUG( ( "Too close, stop" ) );
  res->drive = 0;
}

uint8_t EscapeInstinct::consider( Context *ctx ) {
  DEBUG( ( "drive RMS = %d, turn RMS = %d", ctx->drive.getRMS(), ctx->turn.getRMS() ) );
  if ( ctx->drive.getRMS() < 20 && ctx->turn.getRMS() < 30 ) return 2;
  return 0;
}

void EscapeInstinct::apply( Context *ctx, Insight *res ) {
  int16_t dl = ctx->lprox.getMean();
  int16_t dr = ctx->rprox.getMean();
  int16_t diff = dl - dr;
  DEBUG( ( "dl=%d, dr=%d", dl, dr ) );

  res->turn = ( diff ? sgn( diff ) : 1 ) * 255;
}

// vim:ts=2:sw=2:sts=2:et:ft=cpp
