#include "time.h"

char getDateTime(struct DateTime &dt) {
  char result = Controllino_ReadTimeDate(&dt.day, &dt.weekday, &dt.month, &dt.year, &dt.hour, &dt.minute, &dt.second);
  return result;
}

void printTime(struct DateTime dt) {
  // Serial.println(String(dt.hour) + ":" + String(dt.minute) + ":" + String(dt.second));
}

struct DateTime addDate(struct DateTime baseDate, struct DateTime addDate) {
  struct DateTime result;
  unsigned char carry = 0;
  result.second = baseDate.second + addDate.second;
  if (result.second >= 60) {
    result.second = result.second % 60;
    carry = 1;
  } else {
    carry = 0;
  }

  result.minute = baseDate.minute + addDate.minute + carry;
  if (result.minute >= 60) {
    result.minute = result.minute % 60;
    carry = 1;
  } else {
    carry = 0;
  }

  result.hour = baseDate.hour + addDate.hour + carry;
  if (result.hour >= 24) {
    result.hour = result.hour % 24;
    carry = 1;
  } else {
    carry = 0;
  }

  result.day = baseDate.day + addDate.day + carry;
  if (result.day > daysInMonth[baseDate.month]) {
    result.day = result.day % daysInMonth[baseDate.month];
    carry = 1;
  } else {
    carry = 0;
  }

  result.month = baseDate.month + addDate.month + carry;
  if (result.month > 12) {
    result.month = result.month % 12;
    carry = 1;
  } else {
    carry = 0;
  }

  result.year = (baseDate.year + addDate.year + carry) % 99;

  return result;
}

bool isDateElapsed(struct DateTime current, struct DateTime target) {
  if (current.year > target.year) {
    return 1;
  }

  bool year = (current.year == target.year);

  if (current.month > target.month && year) {
    return 1;
  }

  bool month = (current.month == target.month);

  if (current.day > target.day && month && year) {
    return 1;
  }

  bool day = (current.day == target.day);

  if (current.hour > target.hour && day && month && year) {
    return 1;
  }

  bool hour = (current.hour == target.hour);

  if (current.minute > target.minute && hour && day && month && year) {
    return 1;
  }

  bool minute = (current.minute == target.minute);

  if (current.second > target.second && minute && hour && day && month && year) {
    return 1;
  }

  return 0;
}
