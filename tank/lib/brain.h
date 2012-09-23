/* brain.h */

#ifndef __BRAIN_H
#define __BRAIN_H

#include "instinct.h"

class Brain {
private:
  Context ctx;
  Instinct *instincts;
  Observation <int16_t, 5> sturn, sdrive;
public:
  void addInstinct( Instinct *i );
  void update( int16_t lprox, int16_t rprox, int16_t *turn, int16_t *drive );
};

#endif

// vim:ts=2:sw=2:sts=2:et:ft=cpp
