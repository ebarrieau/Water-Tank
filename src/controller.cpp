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
  struct DateTime startDelay = {0,0,0,0,0,0,30}; //5 minutes
  char dateResult = getDateTime(currentTime);
  if (!dateResult) {
    nextOnTime = addDate(currentTime, startDelay);
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

void Controller::manageWater() {
  if (error) {
    turnWaterOff();
    return;
  }
  if (currentWeight > maxWeight) {
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

  if (isDateElapsed(now, nextOnTime)) {
    turnWaterOn();
  }

}

 float Controller::getWeight() {
  return currentWeight;
}

void Controller::update() {
  updateCurrentWeight();
  manageWater();

  // digitalWrite(pumpPin, (currentWeight >= minPumpWeight));
}
