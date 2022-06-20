#include "SPI.h"
#include "Controllino.h"
#include "time.h"
#include "weight.h"
#include "controller.h"

Controller tank(CONTROLLINO_R2, CONTROLLINO_R1, CONTROLLINO_AI12);


void setup() {

  Serial.begin(19200);
  char rtcInit = Controllino_RTC_init();
  if (!rtcInit) {
    tank.setup();
  }


}

long updatetime = 0;

void loop() {
  tank.update();
  long now = millis();
  if (now - updatetime > 2000) {
    updatetime = now;
    Serial.println(tank.getWeight());
  }
}
