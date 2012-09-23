/* instinct.h */

#ifndef __OBSERVATION_H
#define __OBSERVATION_H

#include <stdint.h>
#include "imath.h"

template <class T, int N>
class Observation {
private:
  T ds[N];
  uint16_t pos, used;
  uint8_t dirty;
  T mean, rms;
private:
  T computeSum();
  void compute();
  void freshen();
public:
  Observation(): pos( 0 ), used( 0 ) {}
  void push( T d );
};

template <class T, int N>
T Observation<T, N>::computeSum( ) {
  T sum = 0;
  uint16_t i;
  for ( i = 0; i < N; i++ )
    sum += ds[i];
  return sum;
}

template <class T, int N>
void Observation<T, N>::compute( ) {
  uint32_t ms = 0;
  uint16_t i;

  mean = computeSum() / N;

  for ( i = 0; i < N; i++ ) {
    T dd = ds[i] - mean;
    ms += dd * dd;
  }

  rms = isqrt( ms / N );
}

template <class T, int N>
void Observation<T, N>::freshen( ) {
  if ( dirty ) {
    compute();
    dirty = 0;
  }
}

template <class T, int N>
void Observation<T, N>::push( T d ) {
  if ( pos == N ) pos = 0;
  ds[pos++] = d;
  if ( pos > used ) used = pos;
  dirty = 1;
}

#endif
/* vim:ts=2:sw=2:sts=2:et:ft=cpp
 */
