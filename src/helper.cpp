#include "helper.h"

float mapf(float inVal, float inLow, float inHigh, float outLow, float outHigh) {
  return (inVal - inLow) / (inHigh - inLow) * (outHigh - outLow) + outLow;
}

void delayForever() {
  while (true) {
    delay(1);
  }
}
