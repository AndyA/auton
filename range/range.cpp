#include "Arduino.h"
#include "Servo.h"

#define LED 13
#define IDX 2
#define SPIN 10

#define DESIRED 550

Servo spin;

unsigned long last_idx;
unsigned long period;
unsigned long count;
unsigned int throttle = 0;

void index_int() {
  unsigned long now = millis();

  period = now - last_idx;
  last_idx = now;
  count++;
}

void setup() {
  Serial.begin(38400);

  pinMode(LED, OUTPUT);
  pinMode(IDX, INPUT);

  spin.attach(SPIN);
  spin.write(throttle);

  attachInterrupt(0, index_int, FALLING);
}

unsigned long ls_millis, ls_count;
unsigned long last_count;

void loop() {
  unsigned long now = millis();

  digitalWrite(LED, digitalRead(IDX) ? LOW : HIGH);

  if (count != last_count) {
    Serial.print(count);
    Serial.print(", ");
    Serial.print(throttle);
    Serial.print(", ");
    Serial.println(period);

    if (now - ls_millis > 20000 || count - ls_count >= 8) {
      throttle++;
      ls_millis = now;
      ls_count = count;
      if (throttle > 180) {
        spin.write(90);
        while (1)
          ;
      }
      spin.write(throttle);
    }
    last_count = count;
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
