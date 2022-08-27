#ifndef WELLMODBUSCLIENT_H
#define WELLMODBUSCLIENT_H

#include "Arduino.h"
#include "Ethernet.h"
#include "ArduinoRS485.h"
#include "ArduinoModbus.h"
#include "Controllino.h"
#include "controller.h"
#include "time.h"

class WellModbusClient
{
public:
  WellModbusClient(Controller *tank,
                   uint8_t unitId,
                   IPAddress ip,
                   uint16_t port = 502);
  void poll(long now);

private:
  Controller *tank;
  uint8_t unitId;
  IPAddress ip;
  uint16_t port;
  struct DateTime pollTime = {0,0,0,0,0,0,2}; // 2 seconds
  long lastPollTime = 0;
  EthernetClient ethClient;
  ModbusTCPClient modbusTcpClient;
  int readDepth(long now);

};

#endif
