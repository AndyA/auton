#!/usr/bin/env perl

use strict;
use warnings;

use Data::Dumper;
use GD;
use JSON;
use Path::Class;

use constant DIR     => 'colcap';
use constant DEV     => "/dev/ttyACM0";
use constant CAMPORT => "usb:001,015";
use constant CAMERA  => "Nikon DSC D80 (PTP mode)";

my $map = read_tiles( DIR );
print JSON->new->pretty->encode( $map );

sub read_tiles {
  my $dir = shift;
  my %map = ();
  for my $r ( 0 .. 15 ) {
    for my $g ( 0 .. 15 ) {
      for my $b ( 0 .. 15 ) {
        my ( $rr, $gg, $bb ) = map { $_ | ( $_ << 4 ) } $r, $g, $b;
        my $rgb = sprintf '%1x%1x%1x', $r, $g, $b;
        my $tile = file $dir, "$rgb.jpg";
        next unless -e $tile;
        $map{$rr}{$gg}{$bb} = read_tile( $tile );
      }
    }
  }
  return \%map;
}

sub read_tile {
  my $fn  = shift;
  my $img = GD::Image->new( "$fn" );
  my ( $mx, $my ) = $img->getBounds;
  my $sx = $mx / 10;
  my $sy = $my / 10;
  my ( $r, $g, $b, $c ) = ( 0, 0, 0, 0 );
  for ( my $x = $sx; $x < $mx; $x += $sx ) {
    for ( my $y = $sy; $y < $my; $y += $sy ) {
      my ( $pr, $pg, $pb ) = $img->rgb( $img->getPixel( $x, $y ) );
      $r += $pr;
      $g += $pg;
      $b += $pb;
      $c++;
    }
  }
  return [ $r / $c, $g / $c, $b / $c ];
}

sub make_colours {
  my @c = ();
  for my $r ( 0 .. 15 ) {
    for my $g ( 0 .. 15 ) {
      for my $b ( 0 .. 15 ) {
        push @c, [ map { $_ | ( $_ << 4 ) } $r, $g, $b ];
      }
    }
  }
  return @c;
}

sub set_colour {
  my $col = shift;
  system './colour', DEV, $col and die "colour failed; $?";
}

sub capture {
  my $img = shift;
  system 'gphoto2', '--port', DEV, '--camera', CAMERA,
   '--capture-image-and-download', '--filename', "$img"
   and die "Capture for $img failed: $?";
}

# vim:ts=2:sw=2:sts=2:et:ft=perl
