#ifndef TANKMODBUSSERVER_H
#define TANKMODBUSSERVER_H

#include "Arduino.h"
#include "Ethernet.h"
#include "ArduinoRS485.h"
#include "ArduinoModbus.h"
#include "controller.h"
#include "Controllino.h"

class TankModbusServer
{
public:
  TankModbusServer(Controller tank, uint8_t mac[6], uint8_t *address);
  void setup();
  void manageModbus();

private:
  Controller tank;
  uint8_t mac[6]; //{0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  IPAddress ip; // ip(192, 168, 1, 177);
  EthernetServer ethServer;
  EthernetClient client;
  ModbusTCPServer modbusTCPServer;

};

#endif
