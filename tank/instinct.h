/* instinct.h */

#ifndef __INSTINCT_H
#define __INSTINCT_H

#include <stdint.h>

template <class T, int N>
class Observation {
private:
  T ds[N];
  uint16_t pos, used;
public:
  Observation();
  void push( T d );
};

class Context {
private:
  Observation<int16_t, 50> turn, drive, range[2];
};

class Insight {
public:
  int16_t turn, drive;
};

class Instinct {
public:
  virtual uint8_t update( const Context *ctx, Insight *res ) = 0;
};

class ExploreInstinct: public Instinct {
public:
  virtual uint8_t update( const Context *ctx, Insight *res );
};

#endif
/* vim:ts=2:sw=2:sts=2:et:ft=cpp
 */
