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
  void setError(char err) {error = err; };
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
  char error = 0;
  uint16_t hysteresis = 500;
  struct DateTime minRechargeTime = {0,0,0,0,1,0,0}; // 1 hour
  struct DateTime maxRunTime = {0,0,0,0,0,5,0}; // 5 minutes
  uint32_t nextOnTime;
  uint32_t nextOffTime;
  struct DateTime pumpTimeout = {0,0,0,0,0,0,30}; // 30 seconds
  uint32_t pumpOffTime;
  uint32_t lastGoodWellTime;
  struct DateTime wellTimeout = {0,0,0,0,0,5,0};
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


// struct Tank {
//   float currentWeight = 2500;
//   float maxWeight = 2500;
//   float incrementalWeight = 125;
//   long recharge = 55;
//   float targetWeight;
//   unsigned int solenoidPin = CONTROLLINO_R2;
//   unsigned int pumpPin = CONTROLLINO_R0;
//   unsigned int scalePin = CONTROLLINO_AI12;
//   DateTime lastOn;
//   bool on = 0;
// }tank;

// struct Well {
//   float currentGallons = 0;
//   float minGallons = 300;
//   unsigned int pumpPin = CONTROLLINO_R1;
//   DateTime lastContact;
// }well;
