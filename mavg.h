/* mavg.h */

#ifndef __MAVG_H
#define __MAVG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#define MAVG_PTS    16
#define MAVG_DTYPE  int
#define MAVG_TTYPE  int

  typedef struct mavg {
    MAVG_TTYPE total;
    MAVG_DTYPE sample[MAVG_PTS];
    unsigned char used, pos;
    struct mavg *par;
  } mavg;

  void mavg_init( mavg *m, mavg *par );
  mavg *mavg_init_multi( mavg *mm, size_t len );
  mavg *mavg_new( size_t len );
  void mavg_free( mavg *m );
  MAVG_DTYPE mavg_update( mavg *m, MAVG_DTYPE d );

#ifdef __cplusplus
}
#endif

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
