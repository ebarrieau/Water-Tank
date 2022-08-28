#include "time.h"

char getDateTimeStruct(struct DateTime &dt) {
  char result = Controllino_ReadTimeDate(&dt.day, &dt.weekday, &dt.month, &dt.year, &dt.hour, &dt.minute, &dt.second);
  return result;
}

char getDateTimeSeconds(uint32_t &time) {
  struct DateTime now;
  char result = getDateTimeStruct(now);
  time = convertDateSeconds(now);
  return result;
}

// void printTime(struct DateTime dt) {
//   Serial.println(String(dt.year) + ":" + String(dt.month) + ":" + String(dt.day));
// }

uint32_t convertDateSeconds(struct DateTime date, bool absolute) {
  // returns seconds since Jan 01, 2000
  uint32_t result = 0;

  if (absolute) {
    bool leapyear = date.year % 4;

    if (date.year > 0) {
      uint8_t yearSinceLeap = (date.year - 1) % 4;

      result += (date.year - yearSinceLeap) * 31557600; //Add 365.25 days in seconds for every full set of leap years
      result += yearSinceLeap * 3153600; //Add 365 days in seconds for ever year in incomplete set of lear years
    }

    for (int i=0; i<date.month; i++) {
      int days = daysInMonth[i];
      if (leapyear && i == 1) { //correct days in february on leap year
        days = 29;
      }
      result += 86400 * days;
    }
  }
  result += date.day * 86400;  // 24 hours in a day
  result += date.hour * 3600;
  result += date.minute * 60;
  result += date.second;
  return result;
}

uint32_t addDate(uint32_t base, uint32_t add) {
  return base + add;
}

uint32_t addDate(uint32_t base, struct DateTime add, bool absolute) {
  return addDate(base, convertDateSeconds(add, absolute));
}

uint32_t addDate(struct DateTime base, struct DateTime add, bool absolute) {
  return addDate(convertDateSeconds(base), add, absolute);
}

bool isDateElapsed(uint32_t current, uint32_t target) {
  return current > target;
}

bool isDateElapsed(uint32_t current, struct DateTime target) {
  return isDateElapsed(current, convertDateSeconds(target));
}

bool isDateElapsed(struct DateTime current, struct DateTime target) {
  return isDateElapsed(convertDateSeconds(current), target);
}
