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
char getDateTimeSeconds(uint32_t &time);
uint32_t convertDateSeconds(struct DateTime date, bool absolute=true);
// void printTime(struct DateTime dt);
uint32_t addDate(uint32_t base, uint32_t add);
uint32_t addDate(uint32_t base, struct DateTime add, bool absolute=true);
uint32_t addDate(struct DateTime base, struct DateTime add, bool absolute=true);
bool isDateElapsed(uint32_t current, uint32_t target);
bool isDateElapsed(uint32_t currrent, struct DateTime target);
bool isDateElapsed(struct DateTime current, struct DateTime target);

#endif
