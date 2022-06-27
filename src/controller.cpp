#include "controller.h"

Controller::Controller(unsigned int solenoidPin,
                       unsigned int pumpPin,
                       unsigned int scalePin) : solenoidPin{solenoidPin},
                                                pumpPin{pumpPin},
                                                scalePin{scalePin}
{


}

void Controller::setup() {
  pinMode(solenoidPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(scalePin, INPUT);

  updateCurrentWeight();
  targetIncWeight = currentWeight + maxIncWeight;
  struct DateTime currentTime;
  struct DateTime startDelay = {0,0,0,0,0,0,30}; //30 seconds
  char dateResult = getDateTime(currentTime);
  if (!dateResult) {
    nextOnTime = addDate(currentTime, startDelay);
    pumpOffTime = currentTime;
  } else {
    error = dateResult;
  }

}

void Controller::updateCurrentWeight() {
  int loadCellRaw = analogRead(CONTROLLINO_AI12);
  currentWeight = convertWeight(loadCellRaw);
}

void Controller::turnWaterOn(struct DateTime now) {
  if (!digitalRead(solenoidPin)) {
    targetIncWeight = currentWeight + currentIncWeightTarget;
    nextOffTime = addDate(now, maxRunTime);
    Serial.print(currentWeight);
    Serial.print(", ");
    Serial.println(targetIncWeight);
  }
  if (error) {
    turnWaterOff(now);
    return;
  }
  digitalWrite(solenoidPin, HIGH);
}

void Controller::turnWaterOff(struct DateTime now) {
  if (digitalRead(solenoidPin)) {
    nextOnTime = addDate(now, minRechargeTime);
  }
  digitalWrite(solenoidPin, LOW);
}

void Controller::turnPumpOn(struct DateTime now) {
  if (digitalRead(pumpPin) && !error) { //if pump is already on and no errors, do nothing
    return;
  }

  if (error) {
    turnPumpOff(now);
    return;
  }

  struct DateTime onDelay = addDate(pumpOffTime, pumpTimeout);
  if (isDateElapsed(now, onDelay)) {
    digitalWrite(pumpPin, HIGH);
  }

}

void Controller::turnPumpOff(struct DateTime now) {
  if (!digitalRead(pumpPin)) { //if pump is alread off, do nothing
    return;
  } else { //if pump is on, set the pump off time for the timeout
    pumpOffTime = now;
  }
  digitalWrite(pumpPin, LOW);
}

void Controller::manageWater(struct DateTime now) {
  if (error) {
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
  }

  if (digitalRead(solenoidPin) && isDateElapsed(now, nextOffTime)) {
    turnWaterOff(now);
    return;
  }

  if (isDateElapsed(now, nextOnTime)) { //only turn on if min recharge time has elapsed
    if (currentWeight < (maxWeight - hysteresis)) { // only turn on if we are well below max volume this helps stop solenoid from short cycling
      struct DateTime wellGoodUntil = addDate(lastGoodWellTime, wellTimeout);
      if (!isDateElapsed(now, wellGoodUntil)) { //if the good until time has not elapsed, we trust the well depth and calculate a target weight
        uint16_t availableWater = (1000 - currentWellDepth) * 1.5 * 8.33; //height between limit and current * gallons/foot * weight/gallon * 10 (fixed point 1 decimal)
        currentIncWeightTarget = min(availableWater, maxIncWeight);
      } else { //Otherwise we use a conservative default of 100.0 lbs
        currentIncWeightTarget = 1000;
      }

      turnWaterOn(now);
    }
  }

}

void Controller::managePump(struct DateTime now) {
  if (error) {
    turnPumpOff(now);
    return;
  }
  if (currentWeight <= minPumpWeight) {
    turnPumpOff(now);
    return;

  } else if (currentWeight > (minPumpWeight + hysteresis)) {
    turnPumpOn(now);
  }
}

void Controller::update(struct DateTime now) {
  updateCurrentWeight();
  manageWater(now);
  managePump(now);
}
