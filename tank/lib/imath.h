/* imath.h */

#ifndef __IMATH_H
#define __IMATH_H

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef clip
#define clip(a, b, c) max(a, min(b, c))
#endif
#ifndef iabs
#define iabs(x) ((x) < 0 ? -(x) : (x))
#endif
#ifndef sgn
#define sgn(x) ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)
#endif
#ifndef countof
#define countof(a) (sizeof(a) / sizeof((a)[0]))
#endif

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

  uint32_t isqrt( uint32_t n );

#ifdef __cplusplus
}
#endif

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
