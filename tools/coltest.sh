#!/bin/bash

dev="/dev/ttyACM0"
camport="usb:001,015"
camera="Nikon DSC D80 (PTP mode)"
dir="colcap"
mkdir -p "$dir"
for r in {0..15}; do
  for g in {0..15}; do
    for b in {0..15}; do
      col=$( printf "%1x%1x%1x" $r $g $b )
      tile="$dir/$col.jpg"
      if [ ! -e "$tile" ]; then
        echo $col
        ./colour "$dev" "#$col"
        gphoto2 --port "$camport" --camera "$camera" \
          --capture-image-and-download --filename "$tile"
      fi
    done
  done
done

# vim:ts=2:sw=2:sts=2:et:ft=sh

