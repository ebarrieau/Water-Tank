#include "weight.h"

float convertWeight(int val) {
  return convertFourToTwenty(val, 0, 5000.0);
}

float convertFourToTwenty(int val, int lowCal, int highCal) {
  return mapf(val, 205, 1023, lowCal, highCal);
}
