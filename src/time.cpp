#include "time.h"

bool isDateElapsed(uint32_t current, uint32_t last, uint32_t target) {
  return (uint32_t)(current - last) > target; //this should fix rollover of millis counter
}
