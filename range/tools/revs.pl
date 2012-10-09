#!/usr/bin/env perl

use strict;
use warnings;

use List::Util qw( min max sum );
use Statistics::Basic qw( stddev );

my %by_servo = ();
while ( <> ) {
  s/\r//g;
  chomp;
  my ( $count, $servo, $period ) = split /\s*,\s*/;
  my $rps = 1000 / $period;
  push @{ $by_servo{$servo} }, $rps;
}

for my $s ( sort { $a <=> $b } keys %by_servo ) {
  my @d = @{ $by_servo{$s} };
  shift @d;    # drop first
  next unless @d;
  my $min = min( @d );
  my $max = max( @d );
  my $avg = sum( @d ) / @d;
  my $std = stddev( @d );
  print join( ',', $s, $min, $max, $avg, $std ), "\n";
}

# vim:ts=2:sw=2:sts=2:et:ft=perl

