#!/bin/bash

ffmpeg -y -f video4linux2 -i /dev/video0 \
  -vcodec mpeg4 -qscale 2 -mbd 2 -f mpegts -r 5 \
  "udp://239.255.12.13:1234?ttl=12&pkt_size=188&buffer_size=8192"

# vim:ts=2:sw=2:sts=2:et:ft=sh
