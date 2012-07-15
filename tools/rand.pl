#!/usr/bin/env perl

use strict;
use warnings;

use Time::HiRes qw( sleep );

while () {
  my $rgb = sprintf '%03x', rand 0x1000000;
  print "#$rgb\n";
  system './colour', '/dev/ttyACM0', "#$rgb";
  sleep 0.5;
}

# vim:ts=2:sw=2:sts=2:et:ft=perl

