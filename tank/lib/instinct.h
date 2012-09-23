/* instinct.h */

#ifndef __INSTINCT_H
#define __INSTINCT_H

#include "observation.h"
#include <stdint.h>

class Context {
public:
  Observation <int16_t, 50> lprox, rprox, turn, drive;
public:
  void update( int16_t lprox, int16_t rprox, int16_t turn, int16_t drive );
  int16_t getProx();
};

class Capacitor {
private:
  int32_t energy;
public:
  void charge( int32_t e );
  void discharge( int16_t *sink, int16_t max );
};

class Insight {
public:
  int16_t turn, drive;
};

class Instinct {
private:
  Instinct *next;
public:
  virtual uint8_t consider( Context *ctx ) = 0;
  virtual void apply( Context *ctx, Insight *res ) = 0;
  void setNext( Instinct *i ) {
    next = i;
  }
  Instinct *getNext() {
    return next;
  }
};

class ExploreInstinct: public Instinct {
public:
  virtual uint8_t consider( Context *ctx );
  virtual void apply( Context *ctx, Insight *res );
};

class SteerInstinct: public Instinct {
private:
  Capacitor cap;
public:
  virtual uint8_t consider( Context *ctx );
  virtual void apply( Context *ctx, Insight *res );
};

#endif

// vim:ts=2:sw=2:sts=2:et:ft=cpp
