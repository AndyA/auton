/* mavg_test.c */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../mavg.h"

#include "tap.h"

typedef struct {
  unsigned len;
  MAVG_DTYPE value;
} seq;

typedef struct seq_list {
  seq s;
  struct seq_list *p;
} seq_list;

typedef struct {
  const char *name;
  int depth;
  seq *in;
  seq *out;
} tcase;

#define RUN(n, v) {n,v}
#define EOL RUN(0, 0)

static tcase tests[] = {
  {
    .name = "Depth: 1, no change",
    .depth = 1,
    .in = ( seq[] ) {
      {
        1000, 12345
      },
      EOL
    },
    .out = ( seq[] ) {
      {
        1000, 12345
      },
      EOL
    }
  },
  {
    .name = "Depth: 5, no change",
    .depth = 5,
    .in = ( seq[] ) {
      {
        1000, 12345
      },
      EOL
    },
    .out = ( seq[] ) {
      {
        1000, 12345
      },
      EOL
    }
  },
  {
    .name = "Depth: 1",
    .depth = 1,
    .in = ( seq[] ) {
      {
        1000, 12345
      },
      { 100, 12355 },
      EOL
    },
    .out = ( seq[] ) {
      {
        1001, 12345
      },
      { 2, 12346 },
      { 1, 12347 } ,
      { 2, 12348 },
      { 1, 12349 },
      { 2, 12350 },
      { 2, 12351 },
      { 1, 12352 },
      { 2, 12353 },
      { 1, 12354 },
      { 85, 12355 },
      EOL
    }
  },
  {
    .name = "Depth: 2",
    .depth = 2,
    .in = ( seq[] ) {
      {
        1000, 12345
      },
      { 400, 12355 },
      EOL
    },
    .out = ( seq[] ) {
      {
        1040, 12345
      },
      { 16, 12346 },
      { 32, 12347 },
      { 16, 12348 },
      { 32, 12349 },
      { 32, 12350 },
      { 16, 12351 },
      { 32, 12352 },
      { 16, 12353 },
      { 32, 12354 },
      { 136, 12355 },
      EOL
    }
  },
  {
    .name = "Depth: 3",
    .depth = 3,
    .in = ( seq[] ) {
      {
        1000, 12345
      },
      { 6400, 12355 },
      EOL
    },
    .out = ( seq[] ) {
      {
        1280, 12345
      },
      { 256, 12346 },
      { 256, 12347 },
      { 256, 12348 },
      { 256, 12349 },
      { 768, 12350 },
      { 1024, 12351 },
      { 256, 12352 },
      { 512, 12353 },
      { 256, 12354 },
      { 2280, 12355 },
      EOL
    }
  },
  {
    .name = "Depth: 4",
    .depth = 4,
    .in = ( seq[] ) {
      {
        1000, 12345
      },
      { 102400, 12355 },
      EOL
    },
    .out = ( seq[] ) {
      {
        4096, 12345
      },
      { 4096, 12348 },
      { 4096, 12350 },
      { 4096, 12351 },
      { 8192, 12352 },
      { 24576, 12353 },
      { 20480, 12354 },
      { 33768, 12355 },
      EOL
    }
  },
};

static size_t seq_llen( const seq_list *sl ) {
  size_t len = 0;
  while ( sl ) {
    len++;
    sl = sl->p;
  }
  return len;
}

static seq *seq_flatten( seq_list *sl ) {
  size_t len = seq_llen( sl );
  seq *s = malloc( sizeof( seq ) * ( len + 1 ) );
  if ( !s ) die( "Out of memory" );
  s[len].len = 0;
  s[len--].value = 0;
  while ( sl ) {
    seq_list *p = sl->p;
    s[len--] = sl->s;
    free( sl );
    sl = p;
  }
  return s;
}

static seq_list *seq_push( seq_list *sl, MAVG_DTYPE d ) {
  if ( sl && sl->s.value == d ) {
    sl->s.len++;
    return sl;
  }

  seq_list *nd = malloc( sizeof( seq_list ) );
  if ( !nd ) die( "Out of memory" );
  nd->s.len = 1;
  nd->s.value = d;
  nd->p = sl;
  return nd;
}

static void diag_seq( const char *pfx, const seq *s ) {
  fprintf( stderr, "# %s: ", pfx );
  while ( s->len ) {
    fprintf( stderr, "{ %u, %ld }, ", s->len, ( long ) s->value );
    s++;
  }
  fprintf( stderr, "{ %u, %ld }\n", s->len, ( long ) s->value );
}

static int cmp_seq( const seq *a, const seq *b ) {
  do  {
    if ( a->value != b->value )
      return a->value < b->value ? -1 : 1;
    if ( a->len != b->len )
      return a->len < b->len ? -1 : 1;
    a++;
    b++;
  }
  while ( a->len );
  return 0;
}

static int is_seq( const seq *got, const seq *want, const char *msg, ... ) {
  va_list ap;
  int _c = 0 == cmp_seq( got, want );
  va_start( ap, msg );
  test( _c, msg, ap );
  va_end( ap );
  if ( !_c ) {
    diag_seq( " got: ", got );
    diag_seq( "want: ", want );
  }
  return _c;
}

static seq *pipe_seq( mavg *m, const seq *in ) {
  seq_list *sl = NULL;
  while ( in->len ) {
    for ( unsigned i = 0; i != in->len; i++ ) {
      MAVG_DTYPE dd = mavg_update( m, in->value );
      sl = seq_push( sl, dd );
    }
    in++;
  }
  return seq_flatten( sl );
}

static void testit( const tcase *tc ) {
  mavg *m = mavg_new( tc->depth );
  seq *got = pipe_seq( m, tc->in );
  is_seq( got, tc->out, "%s: output as expected", tc->name );
  free( got );
  mavg_free( m );
}

int main( void ) {
  int ntest = sizeof( tests ) / sizeof( tests[0] ) ;
  plan( ntest );
  for ( int t = 0; t < ntest; t++ ) {
    testit( &tests[t] );
  }
  return 0;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
