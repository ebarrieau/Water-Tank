#ifndef SIMPLE_TIMER_H
#define SIMPLE_TIMER_H

#include "Arduino.h"

#define MILLIS_IN_SECOND 1000
#define MILLIS_IN_MINUTE 60000
#define MILLIS_IN_HOUR 3600000
#define MILLIS_IN_DAY 86400000

namespace SimpleTimer
{
  void Setup();
  uint32_t Start(uint32_t current, float seconds);
  bool Ended(uint32_t current, uint32_t target);
}


#endif
