//Standard libraries
#include "SPI.h"
#include "Ethernet.h"


//Third Party Libraries
#include "Controllino.h"

//Private Includes
#include "time.h"
#include "weight.h"
#include "controller.h"

Controller tank(CONTROLLINO_R2, CONTROLLINO_R0, CONTROLLINO_AI12);

long updatetime = 0;

void setup() {

  Serial.begin(19200);
  while (!Serial) {
    ; //wait for serial port to connect
  }
  char rtcInit = Controllino_RTC_init();
  if (!rtcInit) {
    tank.setup();
  } else {
    Serial.println("RTC Failed to initialize.");
    delayForever();
  }

}

void loop() {

  if (!client) {
    client = ethServer.available();
    if (client) {
      modbusTCPServer.accept(client);
    }
  }

if (client.connected()) {
  modbusTCPServer.poll();
}

  tank.update();
  long now = millis();
  if (now - updatetime > 2000) {
    updatetime = now;
    Serial.println(tank.getWeight());
  }
}
