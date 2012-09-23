/* instinct.h */

#ifndef __OBSERVATION_H
#define __OBSERVATION_H

#include <stdint.h>

template <class T, int N>
class Observation {
private:
  T ds[N];
  uint16_t pos, used;
public:
  Observation();
  void push( T d );
};

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

#endif
/* vim:ts=2:sw=2:sts=2:et:ft=cpp
 */
