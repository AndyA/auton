/* debug.cpp */

#include "debug.h"
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

static unsigned ts = 0;
static unsigned level = 0;

void db_bump_time( unsigned millis ) {
  ts += millis;
}

void db_set_level( unsigned l ) {
  level = l;
}

static char *fmt_time( char *buf, size_t len, unsigned millis ) {
  snprintf( buf, len, "%02d:%02d:%02d.%03d",
            millis / 1000 / 60 / 60 % 24,
            millis / 1000 / 60 % 60,
            millis / 1000 % 60,
            millis % 1000 );
  return buf;
}

static void __trace( unsigned l, const char *msg, va_list ap ) {
  if ( l <= level ) {
    char tsbuf[20];
    printf( "%s: ", fmt_time( tsbuf, sizeof( tsbuf ), ts ) );
    vprintf( msg, ap );
    printf( "\n" );
  }
}

void __debug( const char *msg, ... ) {
  va_list ap;
  va_start( ap, msg );
  __trace( LL_DEBUG, msg, ap );
  va_end( ap );
}

void __info( const char *msg, ... ) {
  va_list ap;
  va_start( ap, msg );
  __trace( LL_INFO, msg, ap );
  va_end( ap );
}
