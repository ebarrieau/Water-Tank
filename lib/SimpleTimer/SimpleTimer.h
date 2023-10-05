#ifndef SIMPLE_TIMER_H
#define SIMPLE_TIMER_H

#include "Arduino.h"

// #define MILLIS_IN_SECOND 1000
// #define MILLIS_IN_MINUTE 60000
// #define MILLIS_IN_HOUR 3600000
// #define MILLIS_IN_DAY 86400000

namespace SimpleTimer
{
  void Setup(volatile int16_t * pCurrent);
  int16_t Start(int16_t current, int16_t seconds);
  bool Ended(int16_t current, int16_t target);
}

#endif
