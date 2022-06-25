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
  TankModbusServer(Controller *tank,
                   uint8_t mac0,
                   uint8_t mac1,
                   uint8_t mac2,
                   uint8_t mac3,
                   uint8_t mac4,
                   uint8_t mac5,
                   uint8_t address0,
                   uint8_t address1,
                   uint8_t address2,
                   uint8_t address3);
  void setup();
  void poll();

private:
  Controller *tank;
  uint8_t mac[6]; //{0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  IPAddress ip; // ip(192, 168, 1, 177);
  EthernetServer ethServer;
  EthernetClient client;
  ModbusTCPServer modbusTCPServer;
  void updateInputs();

};

// Coils:
// Pump Enable - 00001
// Solenoid Enable - 00002
//
// Discrete Inputs:
// Pump On - 10001
// Solenoid On - 10002
//
// Input Registers:
// Weight - 30001
// Well Depth - 30002
//
// Holding Registers:
// Max Weight - 40001
// Incremental Weight - 40002
// Min Pump Weight - 40003
// Hysteresis Weight - 40004
// Recharge Time - 40005
// Max Run Time - 40006


#endif
