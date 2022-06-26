//Standard libraries
#include "SPI.h"

//Third Party Libraries
#include "Controllino.h"

//Private Includes
#include "network.h"
#include "time.h"
#include "weight.h"
#include "controller.h"
#include "modbusServer.h"
#include "modbusWellClient.h"

Controller tank(CONTROLLINO_R2, CONTROLLINO_R0, CONTROLLINO_AI12);

WellModbusClient wellClient(&tank, IPAddress(10,0,10,11), 502, 11);
TankModbusServer modServer(&tank, 502);


void setup() {

  Serial.begin(19200);
  while (!Serial) {
    ; //wait for serial port to connect
  }
  char rtcInit = Controllino_RTC_init();
  if (!rtcInit) {
    struct NetworkSettings networkSettings = {{0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED},
                                              IPAddress(10,0,10,10)};
    networkSetup(networkSettings);
    tank.setup();
    modServer.setup();
  } else {
    Serial.println("RTC Failed to initialize.");
    delayForever();
  }

}

void loop() {

  struct DateTime now;
  char result = getDateTime(now);
  if (result) {
    tank.setError(result);
  }
  tank.update(now);
  wellClient.poll(now);
  modServer.poll();


}
