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

char getDateTimeStruct(struct DateTime &dt);
char getDateTimeSeconds(long &time);
long convertDateSeconds(struct DateTime date);
// void printTime(struct DateTime dt);
long addDate(long base, long add);
long addDate(long base, struct DateTime add);
long addDate(struct DateTime base, struct DateTime add);
bool isDateElapsed(long current, long target);
bool isDateElapsed(long currrent, struct DateTime target);
bool isDateElapsed(struct DateTime current, struct DateTime target);
// long subtractTime(struct DateTime x, struct DateTime y);

#endif
