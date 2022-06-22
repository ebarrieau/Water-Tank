//Standard libraries
#include "SPI.h"
#include "Ethernet.h"
#include "ArduinoRS485.h"
#include "ArduinoModbus.h"

//Third Party Libraries
#include "Controllino.h"

//Private Includes
#include "time.h"
#include "weight.h"
#include "controller.h"

Controller tank(CONTROLLINO_R2, CONTROLLINO_R0, CONTROLLINO_AI12);

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

EthernetServer ethServer(502);

ModbusTCPServer modbusTCPServer;

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


  Ethernet.init(CONTROLLINO_ETHERNET_CHIP_SELECT);
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    delayForever();
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  ethServer.begin();

  // start the Modbus TCP server
  if (!modbusTCPServer.begin()) {
    Serial.println("Failed to start Modbus TCP Server!");
    delayForever();
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
