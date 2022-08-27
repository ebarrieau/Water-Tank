#ifndef REGRESSION_H
#define REGRESSION_H

#include "Arduino.h"


class Regression
{
public:
  Regression(uint16_t num);
  float getSlope();
  void addPoint(uint16_t val);

private:
  const uint16_t num;
  uint16_t current = 0;
  uint32_t sumX;
  uint32_t sumY;
  uint32_t sumXY;
  uint32_t sumX2;
  uint32_t sumY2;
  uint16_t* y;
};

#endif
