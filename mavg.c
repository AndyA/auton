/* mavg.c */

#include "mavg.h"
#include <stdio.h>
#include <string.h>

void mavg_init( mavg *m, mavg *par ) {
  m->total = 0;
  m->used = 0;
  m->pos = 0;
  m->par = par;
}

mavg *mavg_init_multi( mavg *mm, size_t len ) {
  mavg *p = NULL;
  while ( len ) {
    mavg *m = mm + --len;
    mavg_init( m, p );
    p = m;
  }
  return p;
}

mavg *mavg_new( size_t len ) {
  mavg *m = malloc( sizeof( *m ) * len );
  if ( !m ) return NULL;
  return mavg_init_multi( m, len );
}

void mavg_free( mavg *m ) {
  free( m );
}

static void mavg__push( mavg *m, MAVG_DTYPE d ) {
  if ( m->used == MAVG_PTS ) m->total -= m->sample[m->pos];
  m->sample[m->pos++] = d;
  if ( m->pos > m->used ) m->used = m->pos;
  if ( m->pos == MAVG_PTS ) m->pos = 0;
  m->total += d;
}

MAVG_DTYPE mavg_update( mavg *m, MAVG_DTYPE d ) {
  mavg *mp;
  if ( mp = m->par, mp ) {
    unsigned char opos = mp->pos;
    MAVG_DTYPE dd = mavg_update( mp, d );
    if ( opos == 0 && mp->pos == 1 ) mavg__push( m, dd );
  }
  else {
    mavg__push( m, d );
  }
  return m->total / m->used;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
