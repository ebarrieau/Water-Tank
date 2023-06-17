#include "weight.h"

uint16_t convertWeight(int val) {
  return convertFourToTwenty(val, 0, 50000);
}

uint16_t convertFourToTwenty(int val, uint16_t lowCal, uint16_t highCal) {
  return map(val, 205, 1023, lowCal, highCal);
}
