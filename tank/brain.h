/* brain.h */

#ifndef __BRAIN_H
#define __BRAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

  void br_init( void );
  void br_update( uint16_t lprox, uint16_t rprox, int16_t * ldrive,
                  int16_t * rdrive );

#ifdef __cplusplus
}
#endif
#endif
/* vim:ts=2:sw=2:sts=2:et:ft=cpp
 */
