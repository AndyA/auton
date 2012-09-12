#include <ctype.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <pthread.h>

#include "auton.h"

static uint8_t nb_i[NB_SIZE];
static uint8_t nb_o[NB_SIZE];

static char nb_sig_start[NB_SIG_LEN];
static char nb_sig_end[NB_SIG_LEN];

pthread_mutex_t nb_i_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t nb_o_mtx = PTHREAD_MUTEX_INITIALIZER;

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static const char *device = "/dev/spidev0.0";
static uint8_t mode;
static uint8_t bits = 8;
/*static uint32_t speed = 500000;*/
static uint32_t speed = 250000;
static uint16_t delay;

static void
die( const char *fmt, ... ) {
  va_list ap;
  va_start( ap, fmt );
  vfprintf( stderr, fmt, ap );
  fprintf( stderr, "\n" );
  va_end( ap );
  exit( 1 );
}

static void
hexdump( const void *buf, size_t size ) {
  const uint8_t *bp = buf;
  unsigned i;
  for ( i = 0; i < size; i++ ) {
    printf( "%02x ", bp[i] );
  }
  for ( i = 0; i < size; i++ ) {
    printf( "%c", isprint( bp[i] ) ? bp[i] : '.' );
  }
  printf( "\n" );
}

static void
nb_flip_string( char *buf, const char *str ) {
  while ( *str )
    *buf++ = ~*str++;
}

static void
nb_init( void ) {
  nb_flip_string( nb_sig_start, NB_SIG_START );
  nb_flip_string( nb_sig_end, NB_SIG_END );
}

static void
nb_transfer( int fd ) {
  int ret;
  uint8_t tx[NB_SIG_LEN + NB_SIZE + NB_SIG_LEN];
  uint8_t rx[NB_SIG_LEN + NB_SIZE + NB_SIG_LEN];

  /* build outgoing message */
  memcpy( tx + 0, NB_SIG_START, NB_SIG_LEN );
  pthread_mutex_lock( &nb_i_mtx );
  memcpy( tx + NB_SIG_LEN, nb_i, NB_SIZE );
  pthread_mutex_unlock( &nb_i_mtx );
  memcpy( tx + NB_SIG_LEN + NB_SIZE, NB_SIG_END, NB_SIG_LEN );

  memset( rx, 0, sizeof( rx ) );

  struct spi_ioc_transfer tr = {
    .tx_buf = ( unsigned long ) tx,
    .rx_buf = ( unsigned long ) rx,
    .len = ARRAY_SIZE( tx ),
    .delay_usecs = delay,
    .speed_hz = speed,
    .bits_per_word = bits,
  };

  ret = ioctl( fd, SPI_IOC_MESSAGE( 1 ), &tr );
  if ( ret < 1 )
    die( "can't send spi message" );

  if ( memcmp( rx + 1, nb_sig_start, NB_SIG_LEN ) ||
       memcmp( rx + 1 + NB_SIG_LEN + NB_SIZE, nb_sig_end,
               NB_SIG_LEN - 1 ) ) {
    fprintf( stderr, "Warning: communication error" );
    return;
  }

  pthread_mutex_lock( &nb_o_mtx );
  memcpy( nb_o, rx + 1 + NB_SIG_LEN, NB_SIZE );
  pthread_mutex_unlock( &nb_o_mtx );
}

static void
nb_check_addr( unsigned addr ) {
  if ( addr >= NB_SIZE )
    die( "Noticeboard offset %u out of range (0..%u)", addr, NB_SIZE - 1 );
}

static uint8_t
nb_peek( unsigned addr ) {
  uint8_t v;
  nb_check_addr( addr );
  pthread_mutex_lock( &nb_o_mtx );
  v = nb_o[addr];
  pthread_mutex_unlock( &nb_o_mtx );
  return v;
}

static void
nb_poke( unsigned addr, uint8_t v ) {
  nb_check_addr( addr );
  pthread_mutex_lock( &nb_i_mtx );
  nb_i[addr] = v;
  pthread_mutex_unlock( &nb_i_mtx );
}

int
main( int argc, char *argv[] ) {
  int ret = 0;
  int fd;

  fd = open( device, O_RDWR );
  if ( fd < 0 )
    die( "can't open device" );

  /*
   * spi mode
   */
  ret = ioctl( fd, SPI_IOC_WR_MODE, &mode );
  if ( ret == -1 )
    die( "can't set spi mode" );

  ret = ioctl( fd, SPI_IOC_RD_MODE, &mode );
  if ( ret == -1 )
    die( "can't get spi mode" );

  /*
   * bits per word
   */
  ret = ioctl( fd, SPI_IOC_WR_BITS_PER_WORD, &bits );
  if ( ret == -1 )
    die( "can't set bits per word" );

  ret = ioctl( fd, SPI_IOC_RD_BITS_PER_WORD, &bits );
  if ( ret == -1 )
    die( "can't get bits per word" );

  /*
   * max speed hz
   */
  ret = ioctl( fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed );
  if ( ret == -1 )
    die( "can't set max speed hz" );

  ret = ioctl( fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed );
  if ( ret == -1 )
    die( "can't get max speed hz" );

  nb_init(  );

  nb_transfer( fd );

  close( fd );

  return ret;
}
