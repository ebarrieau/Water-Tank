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
  void update();
    float getWeight();
private:
  float currentWeight;
  float maxWeight = 1000;
  float maxIncWeight = 100;
  float minPumpWeight = 450;
  float targetIncWeight = 0;
  char error = 0;
  struct DateTime minRechargeTime = {0,0,0,0,1,0,0}; // 1 hour
  struct DateTime maxRunTime = {0,0,0,0,0,5,0}; // 5 minutes
  struct DateTime nextOnTime;
  struct DateTime nextOffTime;
  const unsigned int solenoidPin;
  const unsigned int pumpPin;
  const unsigned int scalePin;
  void updateCurrentWeight();
  void turnWaterOn();
  void turnWaterOff();
  void manageWater();

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
