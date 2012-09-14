/* auton.h */

#ifndef __AUTON_H
#define __AUTON_H

#include <stdint.h>

#define NB_SIG_START  "<([{"
#define NB_SIG_END    "}])>"
#define NB_SIG_LEN    4

/* I(nput) and O(utput) are from the PoV of the Arduino */

enum {
  NB_I_DRIVE_LEFT,
  NB_I_DRIVE_RIGHT,
  NB_I_CAM_PAN,
  NB_I_CAM_TILT,
  NB_I_SIZE
};

enum {
  NB_O_OVERRUN,
  NB_O_SIZE
};

#define NB_SIZE \
  ((size_t) NB_I_SIZE > (size_t) NB_O_SIZE ? (size_t) NB_I_SIZE : (size_t) NB_O_SIZE)

typedef void ( *nb_cb_func ) ( uint16_t addr, uint8_t ov, uint8_t nv );

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c 
 */
