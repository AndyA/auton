/* rapi/util.c */

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void
die( const char *fmt, ... ) {
  va_list ap;
  va_start( ap, fmt );
  vfprintf( stderr, fmt, ap );
  fprintf( stderr, "\n" );
  va_end( ap );
  exit( 1 );
}

void
hexdump( const void *buf, size_t size ) {
  const unsigned char *bp = buf;
  unsigned i;
  for ( i = 0; i < size; i++ ) {
    printf( "%02x ", bp[i] );
  }
  for ( i = 0; i < size; i++ ) {
    printf( "%c", isprint( bp[i] ) ? bp[i] : '.' );
  }
  printf( "\n" );
}

void *
alloc( size_t size ) {
  void *m = malloc( size );
  if ( !m )
    die( "Out of memory for %lu bytes", ( unsigned long ) size );
  memset( m, 0, size );
  return m;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c 
 */
