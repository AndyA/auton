#include "Arduino.h"
#include <SPI.h>
#include <Servo.h>
#include <avr/pgmspace.h>

#include "auton.h"

typedef void ( *nb_cb_func ) ( unsigned addr, byte ov, byte nv );

static byte nb_i[NB_SIZE];
static byte nb_o[NB_SIZE];
static byte nb_i_tmp[NB_SIZE];
static nb_cb_func nb_cb[NB_SIZE];
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

static void
nb_register( nb_cb_func cb, unsigned lo, unsigned hi ) {
  unsigned addr;
  for ( addr = lo; addr < hi; addr++ )
    nb_cb[addr] = cb;
}

ISR( SPI_STC_vect ) {
  byte c = SPDR;
  switch ( nb_state ) {
  case NB_PRE:
    if ( ( byte ) nb_sig_start[nb_pos] != c ) {
      nb_gostate( NB_PRE );
      break;
    }
    SPDR = ~( byte ) nb_sig_start[nb_pos];
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
    if ( ( byte ) nb_sig_end[nb_pos] != c ) {
      nb_gostate( NB_PRE );
    }
    SPDR = ~( byte ) nb_sig_end[nb_pos];
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
  unsigned addr;

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
nb_changed( unsigned addr, byte ov, byte nv ) {
  Serial.print( addr );
  Serial.print( " " );
  Serial.print( ov );
  Serial.print( " " );
  Serial.println( nv );
}

void
setup( void ) {
  Serial.begin( 9600 );

  nb_init(  );
  nb_register( nb_changed, 0, NB_SIZE );

  pos = 0;
  process_it = false;

  pinMode( MISO, OUTPUT );

  SPCR |= _BV( SPE ) | _BV( SPIE );
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
