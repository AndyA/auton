#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int
setdtr( int fd, int on ) {
  int controlbits = TIOCM_DTR;
  return ioctl( fd, on ? TIOCMBIC : TIOCMBIS, &controlbits );
}

int
main( int argc, char *argv[] ) {
  if ( argc != 2 ) {
    fprintf( stderr, "Please name a serial port (e.g. /dev/ttyUSB0)\n" );
    return 1;
  }
  const char *dev = argv[1];
  int fd = open( dev, O_RDWR );
  if ( fd < 0 ) {
    fprintf( stderr, "Couldn't open %s\n", dev );
    return 1;
  }

  fprintf( stderr, "Setting DTR\n" );
  setdtr( fd, 1 );

  sleep( 1 );

  fprintf( stderr, "Clearing DTR\n" );
  setdtr( fd, 0 );

  close( fd );
  return 0;
}
