/* mavg.c */

#include "mavg.h"
#include <stdio.h>
#include <string.h>

void
mavg_clear( mavg * m ) {
  m->total = 0;
  m->used = 0;
  m->pos = 0;
}

void
mavg_init( mavg * m, mavg * par ) {
  mavg_clear( m );
  m->par = par;
}

mavg *
mavg_init_multi( mavg * mm, size_t len ) {
  mavg *p = NULL;
  while ( len ) {
    mavg *m = mm + --len;
    mavg_init( m, p );
    p = m;
  }
  return p;
}

mavg *
mavg_new( size_t len ) {
  mavg *m = malloc( sizeof( *m ) * len );
  if ( !m )
    return NULL;
  return mavg_init_multi( m, len );
}

void
mavg_free( mavg * m ) {
  free( m );
}

static void
mavg__push( mavg * m, MAVG_DTYPE d ) {
  if ( m->used == MAVG_PTS )
    m->total -= m->sample[m->pos];
  m->sample[m->pos++] = d;
  if ( m->pos > m->used )
    m->used = m->pos;
  if ( m->pos == MAVG_PTS )
    m->pos = 0;
  m->total += d;
}

void
mavg_reset( mavg * m, MAVG_DTYPE d ) {
  while ( m ) {
    mavg_clear( m );
    mavg__push( m, d );
    m = m->par;
  }
}

MAVG_DTYPE
mavg_peek( mavg * m ) {
  return m->total / m->used;
}

MAVG_DTYPE
mavg_update( mavg * m, MAVG_DTYPE d ) {
  mavg *mp;
  if ( mp = m->par, mp ) {
    uint8_t opos = mp->pos;
    MAVG_DTYPE dd = mavg_update( mp, d );
    if ( opos == 0 && mp->pos == 1 )
      mavg__push( m, dd );
  }
  else {
    mavg__push( m, d );
  }
  return mavg_peek( m );
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
