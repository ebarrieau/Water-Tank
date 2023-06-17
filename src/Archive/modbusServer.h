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
                   uint16_t port);
  void setup();
  void poll();

private:
  Controller *tank;
  EthernetServer ethServer;
  EthernetClient client;
  ModbusTCPServer modbusTCPServer;
  void updateInputs();
  void updateSettings();

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
