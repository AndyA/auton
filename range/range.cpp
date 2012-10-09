#include "Arduino.h"
#include "Servo.h"

#define LED 13
#define IDX 2
#define SPIN 10

Servo spin;

void
setup() {
//  Serial.begin( 38400 );
  pinMode(LED, OUTPUT);
  pinMode(IDX, INPUT);
  spin.attach(SPIN);
  spin.write(90);
}

void
loop() {
  digitalWrite(LED, digitalRead(IDX) ? LOW : HIGH);
//  Serial.print( analogRead( 0 ) );
//  Serial.print( "," );
//  Serial.println( analogRead( 1 ) );
//  delay( 40 );
}

int
main(void) {
  init();
  setup();
  for (;;) {
    loop();
  }
  return 0;
}
