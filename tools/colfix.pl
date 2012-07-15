#!/usr/bin/env perl

use strict;
use warnings;

use Data::Dumper;
use JSON;
use Path::Class;
use List::Util qw( min max sum );

my $mapf = shift or die;
my $cmap = cmap->load( $mapf );
#$cmap->normalise;
#$cmap->each(
#  sub {
#    my ( $in, $out ) = @_;
#    printf "[%f, %f, %f] => [%f, %f, %f]\n", @$in, @$out;
#  }
#);
my $r = $cmap->resolver;
each_col(
  sub {
    my $want = shift;
    my $got  = $r->( $want );
    my ( $in, $out ) = @$got;
    printf "want: [%f, %f, %f], got: [%f, %f, %f] => [%f, %f, %f]\n",
     @$want, @$in, @$out;
  }
);

sub cmap::new {
  my ( $class, $cmap ) = @_;
  return bless {%$cmap}, $class;
}

sub cmap::load {
  my ( $class, $file ) = @_;
  my $cmap = JSON->new->decode( scalar file( $file )->slurp );
  return bless $cmap, $class;
}

sub cmap::each {
  my ( $self, $cb ) = @_;
  for my $r ( sort { $a <=> $b } keys %$self ) {
    for my $g ( sort { $a <=> $b } keys %{ $self->{$r} } ) {
      for my $b ( sort { $a <=> $b } keys %{ $self->{$r}{$g} } ) {
        $cb->( [ $r, $g, $b ], $self->{$r}{$g}{$b} );
      }
    }
  }
}

sub cmap::flatten {
  my $self = shift;
  my @cc   = ();
  $self->each( sub { push @cc, [@_] } );
  return @cc;
}

sub cmap::normalise {
  my $self = shift;
  my @fl   = $self->flatten;
  print Dumper( \@fl );
  my @cc = ();
  for my $c ( 0 .. 2 ) {
    $cc[$c] = [ map { $_->[1][$c] } @fl ];
  }
  print Dumper( \@cc );
}

sub cmap::resolver {
  my $self = shift;
  my @fl   = $self->flatten;
  return sub {
    my $co = shift;
    my @ca = sort { $a->[0] <=> $b->[0] }
     map { [ error( $co, $_->[1] ), $_ ] } @fl;
    return $ca[0][1];
  };
}

sub each_col {
  my $cb = shift;
  for my $r ( 0 .. 15 ) {
    for my $g ( 0 .. 15 ) {
      for my $b ( 0 .. 15 ) {
        $cb->( [ map { $_ | ( $_ << 4 ) } $r, $g, $b ] );
      }
    }
  }
}

sub error {
  my ( $ca, $cb ) = @_;
  my @cw = ( 0.299, 0.587, 0.114 );
  sum map { ( ( $ca->[$_] - $cb->[$_] ) * $cw[$_] )**2 } 0 .. 2;
}

# vim:ts=2:sw=2:sts=2:et:ft=perl

