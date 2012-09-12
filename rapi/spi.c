#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "auton.h"

static unsigned char nb[NB_SIZE];

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void
pabort( const char *s ) {
  perror( s );
  abort(  );
}

static const char *device = "/dev/spidev0.0";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay;

static void
hexdump( const void *buf, size_t size ) {
  const unsigned char *bp = buf;
  unsigned i;
  for ( i = 0; i < size; i++ ) {
    printf( "%02x ", bp[i] );
  }
  printf( "\n" );
}

static void
transfer( int fd ) {
  int ret;
  uint8_t tx[] = {
    0x48, 0x45, 0x4C, 0x4C, 0x4F,
    0x20,
    0x57, 0x4F, 0x52, 0x4C, 0x44,
    0x0A
  };
  uint8_t rx[ARRAY_SIZE( tx )] = { 0, };
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
    pabort( "can't send spi message" );

  printf( "Tx: " );
  hexdump( tx, ARRAY_SIZE( tx ) );
  printf( "Rx: " );
  hexdump( rx, ARRAY_SIZE( rx ) );
}

int
main( int argc, char *argv[] ) {
  int ret = 0;
  int fd;

  fd = open( device, O_RDWR );
  if ( fd < 0 )
    pabort( "can't open device" );

  /*
   * spi mode
   */
  ret = ioctl( fd, SPI_IOC_WR_MODE, &mode );
  if ( ret == -1 )
    pabort( "can't set spi mode" );

  ret = ioctl( fd, SPI_IOC_RD_MODE, &mode );
  if ( ret == -1 )
    pabort( "can't get spi mode" );

  /*
   * bits per word
   */
  ret = ioctl( fd, SPI_IOC_WR_BITS_PER_WORD, &bits );
  if ( ret == -1 )
    pabort( "can't set bits per word" );

  ret = ioctl( fd, SPI_IOC_RD_BITS_PER_WORD, &bits );
  if ( ret == -1 )
    pabort( "can't get bits per word" );

  /*
   * max speed hz
   */
  ret = ioctl( fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed );
  if ( ret == -1 )
    pabort( "can't set max speed hz" );

  ret = ioctl( fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed );
  if ( ret == -1 )
    pabort( "can't get max speed hz" );

  transfer( fd );

  close( fd );

  return ret;
}
