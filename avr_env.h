/* avr_env.h */

#ifndef __AVR_FAKE_H
#define __AVR_FAKE_H

#ifdef ARDUINO

#include "Arduino.h"

#include <avr/pgmspace.h>

#else

#include <string.h>

#define PROGMEM
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define memcpy_P(d, s, l) memcpy(d, s, l);

typedef signed char prog_int8_t;

#endif

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
