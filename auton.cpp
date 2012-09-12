#include "Arduino.h"
#include <SPI.h>
#include <Servo.h>
#include <avr/pgmspace.h>

#include "auton.h"

static byte nb_i[NB_SIZE];
static byte nb_o[NB_SIZE];
static byte nb_i_tmp[NB_SIZE];
static unsigned nb_pos;
static volatile byte nb_state;

static const char nb_sig_start[] = NB_SIG_START;
static const char nb_sig_end[] = NB_SIG_END;

enum { NB_PRE, NB_MSG, NB_POST, NB_DONE };

#define nb_gostate(st) \
  do { nb_pos = 0; nb_state = (st); } while (0)

char buf[100];
volatile byte pos;
volatile boolean process_it;

static void
nb_init(  ) {
  nb_state = NB_PRE;
  nb_pos = 0;
}

void
setup( void ) {
  Serial.begin( 9600 );

  nb_init(  );

  pos = 0;
  process_it = false;

  pinMode( MISO, OUTPUT );

  SPCR |= _BV( SPE ) | _BV( SPIE );
}

ISR( SPI_STC_vect ) {
  byte c = SPDR;
  switch ( nb_state ) {
  case NB_PRE:
    if ( nb_sig_start[nb_pos] != ( char ) c ) {
      nb_gostate( NB_PRE );
      break;
    }
    SPDR = ~nb_sig_start[nb_pos];
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
    if ( nb_sig_end[nb_pos] != ( char ) c ) {
      nb_gostate( NB_PRE );
    }
    SPDR = ~nb_sig_end[nb_pos];
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
  if ( nb_state != NB_DONE )
    return;
  memcpy( nb_i, nb_i_tmp, NB_SIZE );
  nb_gostate( NB_PRE );
}

void
loop( void ) {
  nb_poll(  );
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
