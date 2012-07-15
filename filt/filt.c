/* filt.c */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fft.h"

#define RATE 15625
#define BAND 3

#define RAD(x) ((x) * M_PI / 180.0)

#define eltof(ar) (sizeof(ar) / sizeof((ar)[0]))


static double synth( double f, int pos ) {
  double phase = fmod(( f * 360 / RATE ) * pos, 360 );
  return sin( RAD( phase ) );
}

static void test_filt( double freq, FTYPE out[64] ) {
  FTYPE im[128];
  FTYPE data[128];
  int i;

  for ( i = 0; i < eltof( im ); i++ ) {
    double x = synth( freq, i );
    data[i] = x * 127;
    im[i] = 0;
  }

  fix_fft( data, im, 7, 0 );

  for ( i = 0; i < 64; i++ ) {
    out[i] = ( FTYPE ) sqrt( data[i] * data[i] + im[i] * im[i] );
  }
}

static void to_band( FTYPE data[64], int band[3] ) {
  int step = ( 64 + 2 ) / 3;
  int i, j;
  for ( i = 0; i < 3; i++ ) {
    int tot = 0;
    for ( j = 0; j < step; j++ ) {
      int idx = i * step + j;
      if ( idx < 64 ) {
        tot += data[idx];
      }
    }
    band[i] = tot;
  }
}

int main( void ) {
  FTYPE out[64];
  int band[3];
  double freq;
  int i;

  for ( freq = 20; freq <= RATE / 2; freq *= 1.1 ) {
    test_filt( freq, out );
    to_band( out, band );
    printf( "%d", ( int ) freq );
    for ( i = 0; i < eltof( band ); i++ ) {
      printf( ",%d", band[i] );
    }
    for ( i = 0; i < eltof( out ); i++ ) {
      printf( ",%d", out[i] );
    }
    printf( "\n" );
  }
  return 0;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
