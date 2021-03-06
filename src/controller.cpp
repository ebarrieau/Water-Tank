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
  struct DateTime startDelay = {0,0,0,0,0,10,0}; //30 seconds
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

void Controller::turnWaterOn() {
  if (!digitalRead(solenoidPin)) {
    targetIncWeight = currentWeight + maxIncWeight;
    struct DateTime now;
    char result = getDateTime(now);
    if (!result) {
      nextOffTime = addDate(now, maxRunTime);
    } else {
      error = result;
    }
  }
  if (!error) {
    digitalWrite(solenoidPin, HIGH);
  } else {
    turnWaterOff();
  }
}

void Controller::turnWaterOff() {
  if (digitalRead(solenoidPin)) {
    struct DateTime now;
    char result = getDateTime(now);
    if (!result) {
      nextOnTime = addDate(now, minRechargeTime);
    } else {
      error = result;
    }
  }
  digitalWrite(solenoidPin, LOW);
}

void Controller::turnPumpOn() {
  if (digitalRead(pumpPin) && !error) { //if pump is already on and no errors, do nothing
    return;
  }

  if (!error) {
    struct DateTime now;
    char result = getDateTime(now);
    if (!result) {
      struct DateTime onDelay = addDate(pumpOffTime, pumpTimeout);
      if (isDateElapsed(now, onDelay)) {
        digitalWrite(pumpPin, HIGH);
      }
    } else {
      error = result;
      turnPumpOff();
      return;
    }

  } else {
    turnPumpOff();
  }
}

void Controller::turnPumpOff() {
  if (!digitalRead(pumpPin)) { //if pump is alread off, do nothing
    return;
  } else { //if pump is on, set the pump off time for the timeout
    char result = getDateTime(pumpOffTime);
    if (!result) {
      error = result;
    }
  }
  digitalWrite(pumpPin, LOW);
}

void Controller::manageWater() {
  if (error) {
    turnWaterOff();
    return;
  }
  if (currentWeight >= maxWeight) {
    turnWaterOff();
    return;
  }
  if (digitalRead(solenoidPin) && currentWeight > targetIncWeight) {
    turnWaterOff();
    return;
  }

  struct DateTime now;
  char dateResult = getDateTime(now);
  if (dateResult) {
    error = dateResult;
    return;
  }
  if (digitalRead(solenoidPin) && isDateElapsed(now, nextOffTime)) {
    turnWaterOff();
    return;
  }

  if (isDateElapsed(now, nextOnTime)) { //only turn on if min recharge time has elapsed
    if (currentWeight < (maxWeight - hysteresis)) { // only turn on if we are well below max volume this helps stop solenoid from short cycling
      turnWaterOn();
    }
  }

}

 float Controller::getWeight() {
  return currentWeight;
}

void Controller::managePump() {
  if (error) {
    turnPumpOff();
    return;
  }
  if (currentWeight <= minPumpWeight) {
    turnPumpOff();
    return;
  } else if (currentWeight > (minPumpWeight + hysteresis)) {
    turnPumpOn();
  }
}

void Controller::update() {
  updateCurrentWeight();
  manageWater();
  managePump();
}
