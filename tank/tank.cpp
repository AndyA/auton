#include "Arduino.h"

#include "instinct.h"
#include "brain.h"

#define IN_LF   7
#define IN_LR   8
#define IN_RF   2
#define IN_RR   4

#define OUT_LF  6
#define OUT_LR  9
#define OUT_RF  3
#define OUT_RR  5

#define RANGE_L 1
#define RANGE_R 0

#define CONTROL_MODE 13

#define CHANNEL uint8_t in_f, uint8_t in_r, uint8_t out_f, uint8_t out_r

#define LEFT    IN_LF, IN_LR, OUT_LF, OUT_LR
#define RIGHT   IN_RF, IN_RR, OUT_RF, OUT_RR

static Brain brain;
static ExploreInstinct explore;
static SteerInstinct steer;
static StopInstinct stop;
static EscapeInstinct escape;

static int16_t turn, drive;

static void init_brain() {
  brain.addInstinct( &explore );
  brain.addInstinct( &steer );
  brain.addInstinct( &stop );
  brain.addInstinct( &escape );
}

static void init_channel( CHANNEL ) {
  pinMode( in_f, INPUT );
  pinMode( in_r, INPUT );
  pinMode( out_f, OUTPUT );
  pinMode( out_r, OUTPUT );
}

#define DRIVE_MAX 255
#define DRIVE_MIN 100
#define DRIVE_SCALE(x) \
  ( ( x ) * ( DRIVE_MAX - DRIVE_MIN ) / 255 + DRIVE_MIN )

static int16_t set_channel( CHANNEL, int16_t speed ) {
  speed = max( -255, min( speed, 255 ) );

  if ( speed < 0 ) {
    analogWrite( out_f, 0 );
    analogWrite( out_r, DRIVE_SCALE( -speed ) );
  }
  else {
    analogWrite( out_r, 0 );
    analogWrite( out_f, DRIVE_SCALE( speed ) );
  }
  return speed;
}

void setup( ) {
//  Serial.begin( 38400 );
  pinMode( CONTROL_MODE, INPUT );
  init_channel( LEFT );
  init_channel( RIGHT );
  init_brain();
}

void loop( ) {
  uint16_t lprox = analogRead( RANGE_L );
  uint16_t rprox = analogRead( RANGE_R );

  brain.update( lprox, rprox, &turn, &drive );

  set_channel( LEFT, drive - turn );
  set_channel( RIGHT, drive + turn );

  delay( 40 );
}

int main( void ) {
  init( );
  setup( );
  for ( ;; ) {
    loop( );
  }
  return 0;
}

// vim:ts=2:sw=2:sts=2:et:ft=cpp
