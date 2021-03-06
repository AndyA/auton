#!/usr/bin/env perl

use strict;
use warnings;
use Fcntl;

my ( $fh, $in ) = @ARGV
 ? do {
  my $in = shift;
  sysopen my $fh, $in, O_RDONLY or die "Can't readbytes $in: $!\n";
  ( $fh, $in );
 }
 : ( \*STDIN, 'standard input' );

my @TYPE = qw( QT_RANGE QT_INDEX QT_SPEED );

my %DECODE = (
  QT_RANGE => sub {
    my $buf = shift;
    my ( $val, $chan ) = unpack 'vC', $buf;
    return {
      value   => $val,
      channel => $chan,
      range   => adc_to_range( $val ),
    };
  },
  QT_INDEX => sub {
    return {};
  },
  QT_SPEED => sub {
    my $buf = shift;
    my $speed = unpack 'C', $buf;
    return { speed => $speed, };
  },
);

my %SENSOR = (
  a => 5461,
  b => -17,
  k => 2
);

my %LT = ();

while () {
  my $sync = readbytes( $fh, 1 );
  last unless length $sync;
  next unless $sync eq 'E';
  my $len = read8( $fh );
  next unless $len == 8;
  my $buf = readbytes( $fh, $len );
  my ( $ts, $type ) = unpack 'VC', $buf;
  my $tn = $TYPE[$type];
  unless (defined $tn) {
    print "Bad type: $type\n";
    next;
  }
  my $last = defined $LT{$tn} ? sprintf '(%dms)', $ts - $LT{$tn} : '';
  $LT{$tn} = $ts;
  my $rec = $DECODE{$tn}->( substr $buf, 5 );
  printf "%7d %10s %-9s %s\n", $ts, $last, $tn, join ', ',
   map { "$_=$rec->{$_}" } sort keys %$rec;
}

sub adc_to_range {
  my $r = shift;
  return -1 if $r + $SENSOR{b} <= 0;
  return $SENSOR{a} / ( $r + $SENSOR{b} ) - $SENSOR{k};
}

sub readbytes {
  my ( $fh, $bytes ) = @_;
  my $buf = '';
  while ( length $buf < $bytes ) {
    my $got = sysread $fh, ( my $frag ), $bytes - length $buf;
    die "I/O error: $!\n" unless defined $got;
    return '' if $got == 0;
    $buf .= $frag;
  }
  return $buf;
}

sub read8 { unpack 'C', readbytes( shift, 1 ) }

# vim:ts=2:sw=2:sts=2:et:ft=perl

