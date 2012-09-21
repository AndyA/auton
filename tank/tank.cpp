#include "Arduino.h"

#include "brain.h"

#define IN_LF   7
#define IN_LR   8
#define IN_RF   2
#define IN_RR   4

#define OUT_LF  6
#define OUT_LR  9
#define OUT_RF  3
#define OUT_RR  5

#define RANGE_L 0
#define RANGE_R 1

#define CONTROL_MODE 13

#define CHANNEL uint8_t in_f, uint8_t in_r, uint8_t out_f, uint8_t out_r

#define LEFT    IN_LF, IN_LR, OUT_LF, OUT_LR
#define RIGHT   IN_RF, IN_RR, OUT_RF, OUT_RR
#define FAR     60
#define NEAR    195
#define NEUTRAL 190

static int16_t left, right;
static int16_t speed;

static void
init_channel( CHANNEL ) {
  pinMode( in_f, INPUT );
  pinMode( in_r, INPUT );
  pinMode( out_f, OUTPUT );
  pinMode( out_r, OUTPUT );
}

static int16_t
set_channel( CHANNEL, int16_t speed ) {
  speed = max( -255, min( speed, 255 ) );

  if ( speed < 0 ) {
    analogWrite( out_f, 0 );
    analogWrite( out_r, -speed );
  }
  else {
    analogWrite( out_r, 0 );
    analogWrite( out_f, speed );
  }
  return speed;
}

static void
update_channel( CHANNEL, int16_t * speed, uint16_t prox ) {
  if ( digitalRead( in_f ) ) {
    ( *speed )++;
  }
  else if ( digitalRead( in_r ) ) {
    ( *speed )--;
  }
  else {
    int16_t delta = NEUTRAL - prox;
    if ( *speed < 0 && delta < 0 )
      delta /= 3;
    *speed += delta;
  }

  *speed = max( -255, min( *speed, 255 ) );

  if ( *speed < 0 ) {
    analogWrite( out_f, 0 );
    analogWrite( out_r, -*speed );
  }
  else {
    analogWrite( out_r, 0 );
    analogWrite( out_f, *speed );
  }
}

void
setup(  ) {
  br_init(  );
  pinMode( CONTROL_MODE, INPUT );
  init_channel( LEFT );
  init_channel( RIGHT );
}

void
loop(  ) {
  uint16_t lprox = analogRead( RANGE_R );
  uint16_t rprox = analogRead( RANGE_L );
  int16_t ldrive, rdrive;

  br_update( lprox, rprox, &ldrive, &rdrive );

  set_channel( LEFT, ldrive );
  set_channel( RIGHT, rdrive );

  delay( 40 );
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
