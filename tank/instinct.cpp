#include "instinct.h"

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

void Context::update( int16_t l, int16_t r, int16_t t, int16_t d ) {
  lprox.push( l );
  rprox.push( r );
  turn.push( t );
  drive.push( d );
}

uint8_t ExploreInstinct::consider( const Context *ctx ) {
  return 1;
}

void ExploreInstinct::apply( const Context *ctx, Insight *res ) {
  res->turn = 0;
  res->drive = 255; // hit it!
}
