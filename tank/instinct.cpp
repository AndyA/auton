#include "instinct.h"

template <class T, int N>
Observation<T, N>::Observation() {
  pos = used = 0;
}

template <class T, int N>
void Observation<T, N>::push( T d ) {
  if ( pos == N ) pos = 0;
  ds[pos++] = d;
  if ( pos > used ) used = pos;
}

uint8_t ExploreInstinct::update( const Context *ctx, Insight *res ) {
  return 0;
}
