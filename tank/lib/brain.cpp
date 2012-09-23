/* brain.c */

#include "instinct.h"
#include "brain.h"

#include <stdlib.h>

void Brain::addInstinct( Instinct *i ) {
  i->setNext( instincts );
  instincts = i;
}

void Brain::update( int16_t lprox, int16_t rprox, int16_t *turn, int16_t *drive ) {
  Instinct *i, *best[10];
  Insight res;
  uint8_t best_score = 0, best_used = 0, j;

  ctx.update( lprox, rprox, *turn, *drive );

  for ( i = instincts; i; i = i->getNext() ) {
    uint8_t score = i->consider( &ctx );
    if ( score >= best_score ) {
      if ( score > best_score ) {
        best_score = score;
        best_used = 0;
      }
      if ( best_used < sizeof( best ) / sizeof( best[0] ) ) {
        best[best_used++] = i;
      }
    }
  }

  res.turn = *turn;
  res.drive = *drive;
  for ( j = 0; j < best_used; j++ )
    best[j]->apply( &ctx, &res );
  *turn = res.turn;
  *drive = res.drive;
}

/* vim:ts=2:sw=2:sts=2:et:ft=cpp
 */
