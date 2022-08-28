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

WellModbusClient wellClient(&tank, 11, IPAddress(192,168,1,202), 502);
TankModbusServer modServer(&tank, 502);


void setup() {

  Serial.begin(19200);
  while (!Serial) {
    ; //wait for serial port to connect
  }
  char rtcInit = Controllino_RTC_init();
  if (!rtcInit) {
    struct NetworkSettings networkSettings = {{0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED},
                                              IPAddress(192,168,1,201)};
    networkSetup(networkSettings);
    tank.setup();
    modServer.setup();
  } else {
    // Serial.println("RTC Failed to initialize.");
    delayForever();
  }

}

void loop() {

  uint32_t now;
  char result = getDateTimeSeconds(now);
  if (result) {
    tank.setError(result);
  }
  // Serial.println(now);
  tank.update(now);
  wellClient.poll(now);
  modServer.poll();

  delay(1000);

}
