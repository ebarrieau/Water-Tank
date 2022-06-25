//Standard libraries
#include "SPI.h"
#include "Ethernet.h"


//Third Party Libraries
#include "Controllino.h"

//Private Includes
#include "time.h"
#include "weight.h"
#include "controller.h"
#include "modbusServer.h"

Controller tank(CONTROLLINO_R2, CONTROLLINO_R0, CONTROLLINO_AI12);
TankModbusServer modServer(&tank,
                          0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED,
                          192,168,1,120);

// long updatetime = 0;

void setup() {

  Serial.begin(19200);
  while (!Serial) {
    ; //wait for serial port to connect
  }
  char rtcInit = Controllino_RTC_init();
  if (!rtcInit) {
    tank.setup();
    modServer.setup();
  } else {
    Serial.println("RTC Failed to initialize.");
    delayForever();
  }

}

void loop() {
  
  tank.update();
  modServer.poll();
  // long now = millis();
  // if (now - updatetime > 2000) {
  //   updatetime = now;
  //   Serial.println(tank.getWeight());
  // }
}
