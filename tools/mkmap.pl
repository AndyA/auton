#!/usr/bin/env perl

use strict;
use warnings;

use GD;
use JSON;
use Path::Class;

use constant DIR => 'colcap';
use constant MAP => 'map.json';

my $map = -e MAP ? JSON->new->decode( scalar file( MAP )->slurp ) : {};
read_tiles( $map, DIR );
print { file( MAP )->openw } JSON->new->pretty->encode( $map );

sub read_tiles {
  my ( $map, $dir ) = @_;
  my %map = ();
  for my $r ( 0 .. 15 ) {
    for my $g ( 0 .. 15 ) {
      for my $b ( 0 .. 15 ) {
        my ( $rr, $gg, $bb ) = map { $_ | ( $_ << 4 ) } $r, $g, $b;
        next if exists $map->{$rr}{$gg}{$bb};
        my $rgb = sprintf '%1x%1x%1x', $r, $g, $b;
        my $tile = file $dir, "$rgb.jpg";
        next unless -e $tile;
        $map->{$rr}{$gg}{$bb} = read_tile( $tile );
      }
    }
  }
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

# vim:ts=2:sw=2:sts=2:et:ft=perl

