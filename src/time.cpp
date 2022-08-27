#include "time.h"

char getDateTimeStruct(struct DateTime &dt) {
  char result = Controllino_ReadTimeDate(&dt.day, &dt.weekday, &dt.month, &dt.year, &dt.hour, &dt.minute, &dt.second);
  return result;
}

char getDateTimeSeconds(long &time) {
  struct DateTime now;
  char result = getDateTimeStruct(now);
  time = convertDateSeconds(now);
  return result;
}

// void printTime(struct DateTime dt) {
//   Serial.println(String(dt.year) + ":" + String(dt.month) + ":" + String(dt.day));
// }

long convertDateSeconds(struct DateTime date) {
  // returns seconds since Jan 01, 2000
  // assumes all years are 365.25 days and all months are equal
  long result = 0;
  result += date.year * 1314900; //365.25 days per  year
  result += date.month * 109575; //365.25 days per yer / 12 = 30.4375 days per month
  result += date.day * 86400;  // 24 hours in a day
  result += date.hour * 3600;
  result += date.minute * 60;
  result += date.second;
  return result;
}

long addDate(long base, long add) {
  return base + add;
}

long addDate(long base, struct DateTime add) {
  return addDate(base, convertDateSeconds(add));
}

long addDate(struct DateTime base, struct DateTime add) {
  return addDate(convertDateSeconds(base), add);
}

bool isDateElapsed(long current, long target) {
  return current > target;
}

bool isDateElapsed(long current, struct DateTime target) {
  return isDateElapsed(current, convertDateSeconds(target));
}

bool isDateElapsed(struct DateTime current, struct DateTime target) {
  return isDateElapsed(convertDateSeconds(current), target);
}
