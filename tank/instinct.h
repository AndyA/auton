/* instinct.h */

#ifndef __INSTINCT_H
#define __INSTINCT_H

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

class Context {
private:
  Observation <int16_t, 50> lprox, rprox, turn, drive;
public:
  void update( int16_t lprox, int16_t rprox, int16_t turn, int16_t drive );
};

class Insight {
public:
  int16_t turn, drive;
};

class Instinct {
private:
  Instinct *next;
public:
  virtual uint8_t consider( const Context *ctx ) = 0;
  virtual void apply( const Context *ctx, Insight *res ) = 0;
  void setNext( Instinct *i ) {
    next = i;
  }
  Instinct *getNext() {
    return next;
  }
};

class ExploreInstinct: public Instinct {
public:
  virtual uint8_t consider( const Context *ctx );
  virtual void apply( const Context *ctx, Insight *res );
};

#endif
/* vim:ts=2:sw=2:sts=2:et:ft=cpp
 */
