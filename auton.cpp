#include "Arduino.h"
#include <SPI.h>
#include <Servo.h>
#include <avr/pgmspace.h>

char buf[100];
volatile byte pos;
volatile boolean process_it;

void
setup( void ) {
  Serial.begin( 9600 );

  pos = 0;
  process_it = false;

  pinMode( MISO, OUTPUT );
  SPCR |= _BV( SPE );
  SPI.attachInterrupt(  );
}

ISR( SPI_STC_vect ) {
  byte c = SPDR;
  if ( pos < sizeof buf ) {
    buf[pos++] = c;
    SPDR = ~c;
    if ( c == '\n' )
      process_it = true;
  }
}

void
loop( void ) {
  if ( process_it ) {
    buf[pos] = 0;
    Serial.println( buf );
    pos = 0;
    process_it = false;
  }
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
