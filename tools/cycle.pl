#!/usr/bin/env perl

use strict;
use warnings;

use Device::SerialPort::Arduino;
use Time::HiRes qw( sleep );

sub Device::SerialPort::Arduino::sp { shift->{DSP} }

my $sp = Device::SerialPort::Arduino->new(
  port     => '/dev/ttyACM0',
  baudrate => 9600,
  parity   => 'none',
  databits => 8,
);

$sp->communicate( "#ffffff\n" );
#print wait_msg( $sp ), "\n";
#print wait_msg( $sp ), "\n";

sub wait_msg {
  my $sp  = shift;
  my @msg = ();
  while () {
    my ( undef, $char ) = $sp->sp->read( 1 );
    if ( $char eq '' ) {
      sleep 0.1;
      next;
    }
    print "got: ", ord( $char ), "\n";
    if ( $char =~ /^[\r\n]$/ ) {
      return join '', @msg;
    }
    push @msg, $char;
  }
}

# vim:ts=2:sw=2:sts=2:et:ft=perl

