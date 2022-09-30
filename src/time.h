#ifndef TIME_H
#define TIME_H

#include "Arduino.h"
#include "Controllino.h"

#define MILLIS_IN_SECOND 1000
#define MILLIS_IN_MINUTE 60000
#define MILLIS_IN_HOUR 3600000
#define MILLIS_IN_DAY 86400000

bool isDateElapsed(uint32_t current, uint32_t last, uint32_t target);

#endif
