#!/usr/bin/env perl

use strict;
use warnings;

use Getopt::Long;

my @PRESCALE = ( 1, 8, 64, 256, 1024 );

my %O = ( clock => 16_000_000, );

GetOptions( 'clock:i' => \$O{clock} ) or die;

for my $freq ( @ARGV ) {
  print "Frequency: $freq\n";

  print "PRE  MATCH FREQ\n";
  for my $pre ( @PRESCALE ) {
    my $match = match( $pre, $freq );
    printf "%4d %5d %8.3f\n", $pre, $match, freq( $pre, $match );
  }
}

sub match {
  my ( $pre, $freq ) = @_;
  return int( $O{clock} / ( $pre * $freq ) - 1 + 0.5 );
}

sub freq {
  my ( $pre, $match ) = @_;
  return $O{clock} / ( $pre * ( $match + 1 ) );
}

# vim:ts=2:sw=2:sts=2:et:ft=perl

