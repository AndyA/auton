#include <stdint.h>

#include "Arduino.h"
#include "Servo.h"

#include "queue.h"

#define LED 13
#define IDX 2
#define SPIN 10

#define PRESCALE 256
#define FREQ     200   // sample at 200Hz
#define CHANNELS   3   // three channels
#define ADC_REF    1

Servo spin;

static uint8_t overrun;
static uint8_t channel;
queue q;

#define TMATCH(pre, freq) \
  ((unsigned)(F_CPU / ((long)(pre) * (long)(freq)) - 1 + 0.5))

void setup_timer() {
  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;
  OCR2A = TMATCH(PRESCALE, FREQ * CHANNELS);
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS22) | (1 << CS21); // 256
  TIMSK2 |= (1 << OCIE2A);
  sei();
}

ISR(TIMER2_COMPA_vect) {
  channel++;
  if (channel == CHANNELS) channel = 0;

  ADMUX = (ADC_REF << 6) | channel;
  ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADSC);
}

ISR(ADC_vect) {
  queue_event e;

  e.ts = millis();
  e.type = QT_RANGE;

  e.d.rf.c = channel;
  e.d.rf.r = ADC;

  if (queue_enqueue(&q, &e)) overrun++;
}

void index_int() {
  queue_event e;

  e.ts = millis();
  e.type = QT_INDEX;

  if (queue_enqueue(&q, &e)) overrun++;
}

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(IDX, INPUT);

  spin.attach(SPIN);
  spin.write(88);
//  spin.write(95);

//#define FOO

#ifndef FOO
  attachInterrupt(0, index_int, FALLING);
  setup_timer();
#endif

  queue_empty(&q);
}

void loop() {
  queue_event e;

  digitalWrite(LED, digitalRead(IDX) ? LOW : HIGH);

#ifdef FOO
  delay(100);
  Serial.print(ADCSRA, 2);
  Serial.print(", ");
  Serial.print(ADCSRB, 2);
  Serial.print(", ");
  Serial.print(analogRead(0));
  Serial.print(", ");
  Serial.print(analogRead(1));
  Serial.print(", ");
  Serial.println(analogRead(2));
#endif

  if (queue_deque(&q, &e)) {
    Serial.write("E");
    Serial.write(sizeof(e));
    Serial.write((const uint8_t *) &e, sizeof(e));
  }

  delayMicroseconds(250);
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
