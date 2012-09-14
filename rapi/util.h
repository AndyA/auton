#ifndef __UTIL_H
#define __UTIL_H

#include <stddef.h>

void die( const char *fmt, ... );
void hexdump( const void *buf, size_t size );
void *alloc( size_t size );

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c 
 */
