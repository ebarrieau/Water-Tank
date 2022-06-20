#ifndef TIME_H
#define TIME_H

#include "Arduino.h"
#include "Controllino.h"

struct DateTime {
  unsigned char day;
  unsigned char weekday;
  unsigned char month;
  unsigned char year;
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
};

const uint8_t daysInMonth []  = { 31,28,31,30,31,30,31,31,30,31,30,31 };

char getDateTime(struct DateTime &dt);
void printTime(struct DateTime dt);
struct DateTime addDate(struct DateTime baseDate, struct DateTime addDate);
bool isDateElapsed(struct DateTime current, struct DateTime target);
long subtractTime(struct DateTime x, struct DateTime y);

#endif
