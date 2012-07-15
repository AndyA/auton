#include "avr_env.h"
#include "mavg.h"
#include "fft.h"

#include <stdlib.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define LIM(a, b, c) (MAX( a, MIN( b, c )))

#define ORED 9
#define OGREEN 6
#define OBLUE 5

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

static byte divr;
static volatile byte ibb;

#define BUFHF 128
#define BUFSZ (BUFHF * 2)

#define BANDS 3
#define FFTSZ (BUFHF/2)
#define BANDSZ ((int)((FFTSZ+BANDS-1)/BANDS))
#define SCALE(x) (x)

static FTYPE im[BUFHF];
static FTYPE data[BUFSZ];
static volatile unsigned char dp;
static unsigned char half;

void setup() {
  pinMode( ORED, OUTPUT );
  pinMode( OGREEN, OUTPUT );
  pinMode( OBLUE, OUTPUT );

  cbi( ADCSRA, ADPS2 );
  sbi( ADCSRA, ADPS1 );
  sbi( ADCSRA, ADPS0 );
  sbi( ADMUX, ADLAR ); // 8-Bit ADC in ADCH Register
  sbi( ADMUX, REFS0 ); // VCC Reference
  cbi( ADMUX, REFS1 );
  cbi( ADMUX, MUX0 ); // Set Input Multiplexer to Channel 0
  cbi( ADMUX, MUX1 );
  cbi( ADMUX, MUX2 );
  cbi( ADMUX, MUX3 );
  //
  // Timer2 PWM Mode set to fast PWM
  cbi( TCCR2A, COM2A0 );
  sbi( TCCR2A, COM2A1 );
  sbi( TCCR2A, WGM20 );
  sbi( TCCR2A, WGM21 );

  cbi( TCCR2B, WGM22 );

  sbi( TCCR2B, CS20 );
  cbi( TCCR2B, CS21 );
  cbi( TCCR2B, CS22 );

  sbi( DDRB, 3 );                 // set digital pin 11 to output

  cbi( TIMSK0, TOIE0 );            // disable Timer0 !!! delay is off now
  sbi( TIMSK2, TOIE2 );            // enable Timer2 Interrupt
  Serial.begin( 9600 );
}

ISR( TIMER2_OVF_vect ) {
  divr++;
  if (( divr & 7 ) == 0 ) {
    data[dp++] = ADCH; // only works when dp is an unsigned char
    ibb++;
    ibb--;
    ibb++;
    ibb--;
    sbi( ADCSRA, ADSC );
  }
}

void loop( void ) {
  int i;
  int band[BANDS];

  if ((( half ^ dp ) & BUFHF ) == 0 ) {
    half = ( dp ^ BUFHF ) & BUFHF;
    FTYPE *dd = data + half;
    fix_fft( dd, im, 7, 0 );
    for ( i = 0; i < FFTSZ; i++ ) {
      int b = i / BANDSZ;
      if ( b < BANDS ) {
        int x = dd[i] * dd[i] + im[i] * im[i];
        band[b] = x;
      }
    }
    analogWrite( OBLUE, LIM( 0, SCALE( band[0] ) / 3, 255 ) );
    analogWrite( OGREEN, LIM( 0, SCALE( band[1] ), 255 ) );
    analogWrite( ORED, LIM( 0, SCALE( band[2] ), 255 ) );
  }
}

int main( void ) {
  init();
  setup();
  for ( ;; ) {
    loop();
  }
  return 0;
}

