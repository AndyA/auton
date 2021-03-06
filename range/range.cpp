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

#define MIN_IDX  (1000/4)

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
  static uint32_t last_ts;

  e.ts = millis();
  if (e.ts - last_ts > MIN_IDX) {
    e.type = QT_INDEX;
    if (queue_enqueue(&q, &e)) overrun++;
    last_ts = e.ts;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(IDX, INPUT);

  spin.attach(SPIN);
  spin.write(88);
//  spin.write(97);

  attachInterrupt(0, index_int, FALLING);
  setup_timer();

  queue_empty(&q);
}

static void send_event(queue_event *e) {
  Serial.write("E");
  Serial.write(sizeof(*e));
  Serial.write((const uint8_t *) e, sizeof(*e));
}

static void set_speed(uint16_t sp) {
  queue_event e;
  e.ts = millis();
  e.type = QT_SPEED;
  e.d.spd.speed = sp;
  send_event(&e);
  spin.write(sp);
}

void calibration_loop() {
  queue_event e;
  uint8_t ls = 0;
  for (int sp = 0; sp <= 180; sp++) {
    uint32_t start = millis();
    uint8_t revs = 0;
    set_speed(sp);
    while (revs < 6) {
      if (millis() - start > 10000) break;
      delayMicroseconds(250);
      if (queue_deque(&q, &e)) {
        send_event(&e);
        if (e.type == QT_INDEX) {
          revs++;
          ls = !ls;
          digitalWrite(LED, ls);
        }
      }
    }
  }
}

void loop() {
  queue_event e;

  if (queue_deque(&q, &e))
    send_event(&e);

  // queue_deque disables interrupts - so we need to make sure it
  // isn't called all the time.
  delayMicroseconds(250);
}

int
main(void) {
  init();
  setup();
  for (;;) {
//    calibration_loop();
    loop();
  }
  return 0;
}
