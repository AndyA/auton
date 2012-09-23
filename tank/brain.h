/* brain.h */

#ifndef __BRAIN_H
#define __BRAIN_H

#include "instinct.h"

class Brain {
private:
  Context ctx;
  Instinct *instincts;
public:
  void addInstinct( Instinct * i );
  void update( int16_t lprox, int16_t rprox, int16_t *turn, int16_t *drive);
};

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
