/* brain.c */

#include "Arduino.h"

#include "instinct.h"
#include "brain.h"

#include <stdlib.h>

void Brain::addInstinct( Instinct *i ) {
  i->setNext( instincts );
  instincts = i;
}

void Brain::update( int16_t lprox, int16_t rprox, int16_t *turn, int16_t *drive ) {
  Instinct *i, *best = 0;
  Insight res;
  uint8_t best_score = 0;

  ctx.update( lprox, rprox, *turn, *drive );

  for ( i = instincts; i; i = i->getNext() ) {
    uint8_t score = i->consider( &ctx );
    if ( score > best_score ) {
      best_score = score;
      best = i;
    }
  }

  res.turn = *turn;
  res.drive = *drive;
  i->apply( &ctx, &res );
  *turn = res.turn;
  *drive = res.drive;
}

/* vim:ts=2:sw=2:sts=2:et:ft=cpp
 */
