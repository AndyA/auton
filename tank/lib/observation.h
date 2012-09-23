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
  T mean, rms, acrms, min, max;
private:
  T computeSum();
  void compute();
  void freshen();
public:
  Observation(): pos( 0 ), used( 0 ) {}
  void push( T d );
  T peek( int16_t ofs );
  T getMean() {
    freshen();
    return mean;
  }
  T getRMS() {
    freshen();
    return rms;
  }
  T getACRMS() {
    freshen();
    return acrms;
  }
  T getMin() {
    freshen();
    return min;
  }
  T getMax() {
    freshen();
    return max;
  }
  T getDelta() {
    return peek( -1 ) - getMean();
  }
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
  uint32_t ms = 0, acms = 0;
  uint16_t i;

  mean = computeSum() / N;

  for ( i = 0; i < N; i++ ) {
    T acdd = ds[i] - mean;
    acms += acdd * acdd;
    ms += ds[i] * ds[i];
    if ( i == 0 ) {
      min = max = ds[i];
    }
    else {
      if ( ds[i] < min ) min = ds[i];
      if ( ds[i] > max ) max = ds[i];
    }
  }

  rms = isqrt( ms / N );
  acrms = isqrt( acms / N );
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

template <class T, int N>
T Observation<T, N>::peek( int16_t ofs = 0 ) {
  int16_t idx = pos + ofs;
  while ( idx >= used ) idx -= used;
  while ( idx < 0 ) idx += used;
  return ds[idx];
}

#endif

// vim:ts=2:sw=2:sts=2:et:ft=cpp
