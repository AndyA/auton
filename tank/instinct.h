/* instinct.h */

#ifndef __INSTINCT_H
#define __INSTINCT_H

#include "observation.h"
#include <stdint.h>

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

class SteerInstinct: public Instinct {
public:
  virtual uint8_t consider( const Context *ctx );
  virtual void apply( const Context *ctx, Insight *res );
};

#endif
/* vim:ts=2:sw=2:sts=2:et:ft=cpp
 */
