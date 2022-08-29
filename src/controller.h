#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Arduino.h"
#include "Controllino.h"
#include "time.h"
#include "weight.h"

class Controller
{
public:
  Controller(unsigned int solenoidPin, unsigned int pumpPin, unsigned int scalePin);
  void setup();
  void update(uint32_t now);
  uint16_t getWeight() { return currentWeight; };
  bool getPumpState() { return digitalRead(pumpPin); };
  bool getSolenoidState() { return digitalRead(solenoidPin); };
  uint16_t getTargetWeight() { return targetIncWeight; };
  void setWellDepth(uint16_t depth) { currentWellDepth = depth;};
  float getWellDepth() {return currentWellDepth; };
  void setWellGoodTime(uint32_t time) { lastGoodWellTime = time; };
  uint32_t getWellGoodTime() { return lastGoodWellTime; };
private:
  uint16_t currentWeight;
  uint16_t currentWellDepth;
  uint16_t maxWeight = 35000; //3,500.0 - first digit represents one decimal place
  uint16_t maxIncWeight = 1000;
  uint16_t currentIncWeightTarget;
  uint16_t minPumpWeight = 5000;
  uint16_t targetIncWeight = 0;
  uint16_t hysteresis = 500;
  uint32_t minRechargeTime = 1 * MILLIS_IN_HOUR; // 1 hour
  uint32_t maxRunTime = 5 * MILLIS_IN_MINUTE; // 5 minutes
  uint32_t tankOffTime;
  uint32_t tankOnTime;
  uint32_t pumpTimeout = 30 * MILLIS_IN_SECOND; // 30 seconds
  uint32_t pumpOffTime;
  uint32_t lastGoodWellTime;
  uint32_t wellTimeout = 5 * MILLIS_IN_MINUTE;
  const unsigned int solenoidPin;
  const unsigned int pumpPin;
  const unsigned int scalePin;
  void updateCurrentWeight();
  void turnWaterOn(uint32_t now);
  void turnWaterOff(uint32_t now);
  void turnPumpOn(uint32_t now);
  void turnPumpOff(uint32_t now);
  void managePump(uint32_t now);
  void manageWater(uint32_t now);

};
#endif
