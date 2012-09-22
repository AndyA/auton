#include "Arduino.h"

void
setup(  ) {
  Serial.begin( 38400 );
}

void
loop(  ) {
  Serial.print( analogRead( 0 ) );
  Serial.print( "," );
  Serial.println( analogRead( 1 ) );
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
