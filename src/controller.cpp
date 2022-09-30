#include "controller.h"

Controller::Controller(unsigned int solenoidPin,
                       unsigned int pumpPin,
                       unsigned int scalePin,
                       unsigned int wellPin) : solenoidPin{solenoidPin},
                                                pumpPin{pumpPin},
                                                scalePin{scalePin},
                                                wellPin{wellPin}
{


}

void Controller::setup() {
  pinMode(solenoidPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(scalePin, INPUT);

  updateCurrentWeight();
  targetIncWeight = currentWeight + maxIncWeight;
  uint32_t currentTime = millis();

  tankOffTime = currentTime;
  pumpOffTime = currentTime;

}

void Controller::updateCurrentWeight() {
  int loadCellRaw = analogRead(CONTROLLINO_AI12);
  currentWeight = convertWeight(loadCellRaw);
}

void Controller::turnWaterOn(uint32_t now) {
  if (!digitalRead(solenoidPin)) {
    targetIncWeight = currentWeight + currentIncWeightTarget;
    tankOnTime = now;
  }

  turnWellOn();
  digitalWrite(solenoidPin, HIGH);
}

void Controller::turnWaterOff(uint32_t now) {
  if (digitalRead(solenoidPin)) {
    tankOffTime = now;
  }
  digitalWrite(solenoidPin, LOW);
}

void Controller::turnPumpOn(uint32_t now) {
  if (digitalRead(pumpPin)) { //if pump is already on and no errors, do nothing
    return;
  }

  if (isDateElapsed(now, pumpOffTime, pumpTimeout)) {
    digitalWrite(pumpPin, HIGH);
  }

}

void Controller::turnPumpOff(uint32_t now) {
  if (!digitalRead(pumpPin)) { //if pump is alread off, do nothing
    return;
  } else { //if pump is on, set the pump off time for the timeout
    pumpOffTime = now;
  }
  digitalWrite(pumpPin, LOW);
}

void Controller::turnWellOn() {
  //well is on a NC contactor, so LOW is ON
  if (!digitalRead(wellPin)) { //if well is already on, do nothing
    return;
  }
  digitalWrite(pumpPin, LOW);
}

void Controller::turnWellOff() {
  //well is on a NC contactor, so HIGH is OFF
  if (digitalRead(wellPin)) { //if pump is alread off, do nothing
    return;
  }
  digitalWrite(wellPin, HIGH);
}

void Controller::manageWater(uint32_t now) {

  if (!runState) {
    turnWaterOff(now);
    return;
  }

  if (currentWeight >= maxWeight + hysteresis) {
    turnWellOff();
    turnWaterOff(now);
    return;
  }

  if (currentWeight >= maxWeight) {
    turnWaterOff(now);
    return;
  }
  if (digitalRead(solenoidPin) && currentWeight > targetIncWeight) {
    turnWaterOff(now);
    return;
  }

  if (currentWellDepth > 1000) {
    turnWaterOff(now);
    return;
  } else if (currentWellDepth > 2000) {
    turnWellOff();
    turnWaterOff(now);
  }

  if (digitalRead(solenoidPin) && isDateElapsed(now, tankOnTime, currentRunTime)) {
    turnWaterOff(now);
    return;
  }

  if (isDateElapsed(now, tankOffTime, minRechargeTime)) { //only turn on if min recharge time has elapsed
    if (currentWeight < (maxWeight - hysteresis)) { // only turn on if we are well below max volume this helps stop solenoid from short cycling
      if (!isDateElapsed(now, lastGoodWellTime, wellTimeout)) { //if the good until time has not elapsed, we trust the well depth and calculate a target weight
        uint16_t availableWater = (1000 - currentWellDepth) * 1.5 * 8.33; //height between limit and current * gallons/foot * weight/gallon * 10 (fixed point 1 decimal)
        currentIncWeightTarget = min(availableWater, maxIncWeight);

      } else { //Otherwise we use a conservative default of 100.0 lbs
        currentIncWeightTarget = 1000;
      }
      uint32_t expectedRunTime = currentIncWeightTarget / (5 * 8.33 * 10) * MILLIS_IN_MINUTE; // Expected milliseconds to run target lbs / ( flowrate gpm * lbs/gal)
      currentRunTime = min(maxRunTime, expectedRunTime * 1.2);
      turnWaterOn(now);
    }
  }


}

void Controller::managePump(uint32_t now) {

  if (currentWeight <= minPumpWeight) {
    turnPumpOff(now);
    return;
  } else if (currentWeight > (minPumpWeight + hysteresis)) {
    turnPumpOn(now);

  }
}

void Controller::update(uint32_t now) {
  updateCurrentWeight();
  manageWater(now);
  managePump(now);
}
