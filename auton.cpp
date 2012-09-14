#include "Arduino.h"
#include <Servo.h>
#include <avr/pgmspace.h>

#include "PWM.h"

#include "noticeboard.h"

#define SERVOMIN  150
#define SERVOMAX  700

PWM pwm = PWM(  );

static uint8_t nb_i[NB_SIZE];
static uint8_t nb_o[NB_SIZE];
static uint8_t nb_i_tmp[NB_SIZE];
static uint16_t nb_pos;
static volatile uint8_t nb_state;

static const char nb_sig_start[] = NB_SIG_START;
static const char nb_sig_end[] = NB_SIG_END;

enum { NB_PRE, NB_MSG, NB_POST, NB_DONE };

#define nb_gostate(st) \
  do { nb_pos = 0; nb_state = (st); } while (0)

static void
nb_init(  ) {
  nb_state = NB_PRE;
  nb_pos = 0;
}

ISR( SPI_STC_vect ) {
  uint8_t c = SPDR;
  switch ( nb_state ) {
  case NB_PRE:
    if ( ( uint8_t ) nb_sig_start[nb_pos] != c ) {
      nb_gostate( NB_PRE );
      break;
    }
    SPDR = ~( uint8_t ) nb_sig_start[nb_pos];
    if ( ++nb_pos == NB_SIG_LEN ) {
      nb_gostate( NB_MSG );
    }
    break;
  case NB_MSG:
    nb_i_tmp[nb_pos] = c;
    SPDR = nb_o[nb_pos];
    if ( ++nb_pos == NB_SIZE ) {
      nb_gostate( NB_POST );
      break;
    }
    break;
  case NB_POST:
    if ( ( uint8_t ) nb_sig_end[nb_pos] != c ) {
      nb_gostate( NB_PRE );
    }
    SPDR = ~( uint8_t ) nb_sig_end[nb_pos];
    if ( ++nb_pos == NB_SIG_LEN ) {
      nb_gostate( NB_DONE );
    }
    break;
  case NB_DONE:
    nb_o[NB_O_OVERRUN]++;
    break;
  }
}

static void
nb_poll(  ) {
  uint16_t addr;

  if ( nb_state != NB_DONE )
    return;

  /* trigger callbacks, update primary input array */
  for ( addr = 0; addr < NB_SIZE; addr++ ) {
    if ( nb_i_tmp[addr] != nb_i[addr] ) {
      if ( nb_cb[addr] )
        nb_cb[addr] ( addr, nb_i[addr], nb_i_tmp[addr] );
      nb_i[addr] = nb_i_tmp[addr];
    }
  }

  nb_gostate( NB_PRE );
}

static void
nb_changed( uint16_t addr, uint8_t ov, uint8_t nv ) {
  Serial.print( addr );
  Serial.print( ": " );
  Serial.print( ov );
  Serial.print( " -> " );
  Serial.println( nv );
}

static void
pwm_set( uint8_t addr, uint8_t v ) {
  pwm.setPWM( addr, 0,
              ( uint32_t ) v * ( SERVOMAX - SERVOMIN ) / 255 + SERVOMIN );
}

static void
pwm_changed( uint16_t addr, uint8_t ov, uint8_t nv ) {
  pwm_set( addr - NB_I_CAM_PAN, nv );
}

void
setup( void ) {
  int i;
  Serial.begin( 9600 );

  pwm.begin(  );

  pwm.setPWMFreq( 60 );
  for ( i = 0; i < 16; i++ ) {
    pwm_set( i, i + i * 16 );
  }

  nb_init(  );
  nb_register( nb_changed, 0, NB_SIZE - 1 );
  nb_register( pwm_changed, NB_I_SERVO_START, NB_I_SERVO_END );

  pinMode( MISO, OUTPUT );

  SPCR |= _BV( SPE ) | _BV( SPIE );
}

static unsigned long foo;

void
loop( void ) {
  nb_poll(  );
  nb_o[NB_O_FOO] = foo++ >> 18;
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
