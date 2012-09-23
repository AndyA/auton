/* common.h */

#ifndef __COMMON_H
#define __COMMON_H

#ifdef _DEBUG
void __debug( const char *msg, ... );
void __info( const char *msg, ... );
#define DEBUG(x) __debug x
#define INFO(x) __info x
#else
#define DEBUG(x)
#define INFO(x)
#endif

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
