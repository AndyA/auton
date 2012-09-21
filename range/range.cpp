#include "Arduino.h"

void
setup(  ) {
//  Serial.begin( 9600 );
}

void
loop(  ) {
//  Serial.print( analogRead( 0 ) );
//  Serial.print( ", " );
//  Serial.println( analogRead( 1 ) );
  tone( 8, analogRead( 0 ) );
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
