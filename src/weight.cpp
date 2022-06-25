#include "weight.h"

uint16_t convertWeight(int val) {
  return map(val, 205, 1023, 0, 50000);
}

// float convertWeight(int val) {
//   return convertFourToTwenty(val, 0, 5000.0);
// }
//
// float convertFourToTwenty(int val, int lowCal, int highCal) {
//   return mapf(val, 205, 1023, lowCal, highCal);
// }
