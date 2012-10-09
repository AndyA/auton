#include "Arduino.h"
#include "Servo.h"

#define LED 13
#define IDX 2
#define SPIN 10

#define DESIRED 550

Servo spin;

unsigned long last_idx;
unsigned long period;
unsigned int throttle = 100;

unsigned long last_period, last_throttle;

void index_int() {
  unsigned long now = millis();

  period = now - last_idx;
  last_idx = now;

  if (period > DESIRED && throttle < 180)
    throttle++;
  else if (period < DESIRED && throttle > 0)
    throttle--;

  spin.write(throttle);
}

void setup() {
  Serial.begin(38400);
  pinMode(LED, OUTPUT);
  pinMode(IDX, INPUT);
  spin.attach(SPIN);
  spin.write(throttle);
  attachInterrupt(0, index_int, FALLING);
}

void loop() {
  digitalWrite(LED, digitalRead(IDX) ? LOW : HIGH);
  if (last_period != period || last_throttle != throttle) {
    Serial.print(period);
    Serial.print(", ");
    Serial.println(throttle);
    last_period = period;
    last_throttle = throttle;
  }
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
