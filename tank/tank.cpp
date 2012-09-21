#include "Arduino.h"

int ledPin = 13;

#define IN_LF   7
#define IN_LR   8
#define IN_RF   2
#define IN_RR   4
#define OUT_LF  6
#define OUT_LR  9
#define OUT_RF  3
#define OUT_RR  5

#define CHANNEL uint8_t in_f, uint8_t in_r, uint8_t out_f, uint8_t out_r
#define LEFT    IN_LF, IN_LR, OUT_LF, OUT_LR
#define RIGHT   IN_RF, IN_RR, OUT_RF, OUT_RR

static int16_t left, right;

static void
init_channel( CHANNEL ) {
  pinMode( in_f, INPUT );
  pinMode( in_r, INPUT );
  pinMode( out_f, OUTPUT );
  pinMode( out_r, OUTPUT );
}

static void
update_channel( CHANNEL, int16_t * speed ) {
  if ( digitalRead( in_f ) && *speed < 255 ) {
    ( *speed )++;
  }
  else if ( digitalRead( in_r ) && *speed > -255 ) {
    ( *speed )--;
  }
  else if ( *speed < 0 ) {
    ( *speed )++;
  }
  else if ( *speed > 0 ) {
    ( *speed )--;
  }

  if ( *speed < 0 ) {
    analogWrite( out_f, 0 );
    analogWrite( out_r, -( *speed ) );
  }
  else {
    analogWrite( out_r, 0 );
    analogWrite( out_f, *speed );
  }
}

void
setup(  ) {
  init_channel( LEFT );
  init_channel( RIGHT );
}

void
loop(  ) {
  update_channel( LEFT, &left );
  update_channel( RIGHT, &right );
  delay( 3 );
}

int
main( void ) {
  init(  );
  setup(  );
  for ( ;; ) {
    loop(  );
  }
  return 0;
}
