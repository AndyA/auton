#include "noticeboard.h"


nb_cb_func nb_cb[NB_SIZE];

void
nb_register( nb_cb_func cb, uint16_t lo, uint16_t hi ) {
  uint16_t addr;
  if ( hi < lo ) {
    uint16_t t = lo;
    lo = hi;
    hi = t;
  }
  for ( addr = lo; addr <= hi; addr++ )
    nb_cb[addr] = cb;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c 
 */
