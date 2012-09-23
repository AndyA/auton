/* imath.c */

#include "imath.h"

uint32_t isqrt( uint32_t n ) {
  uint32_t root = 0, bit, trial;
  bit = ( n >= 0x10000 ) ? 1 << 30 : 1 << 14;
  do {
    trial = root + bit;
    if ( n >= trial ) {
      n -= trial;
      root = trial + bit;
    }
    root >>= 1;
    bit >>= 2;
  }
  while ( bit );
  return root;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
