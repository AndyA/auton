/* mavg.h */

#ifndef __MAVG_H
#define __MAVG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

#define MAVG_PTS    8
#define MAVG_DTYPE  int32_t
#define MAVG_TTYPE  int32_t

  typedef struct mavg {
    MAVG_TTYPE total;
    MAVG_DTYPE sample[MAVG_PTS];
    uint8_t used, pos;
    struct mavg *par;
  } mavg;

  void mavg_clear( mavg * m );
  void mavg_init( mavg * m, mavg * par );
  void mavg_reset( mavg * m, MAVG_DTYPE d );
  mavg *mavg_init_multi( mavg * mm, size_t len );
  mavg *mavg_new( size_t len );
  void mavg_free( mavg * m );
  MAVG_DTYPE mavg_peek( mavg * m );
  MAVG_DTYPE mavg_update( mavg * m, MAVG_DTYPE d );

#ifdef __cplusplus
}
#endif
#endif
/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
