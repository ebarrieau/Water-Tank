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
  void poll(uint32_t now);

private:
  Controller *tank;
  uint8_t unitId;
  IPAddress ip;
  uint16_t port;
  uint32_t pollTime = 2 * MILLIS_IN_SECOND; // 2 seconds
  uint32_t lastPollTime = 0;
  EthernetClient ethClient;
  ModbusTCPClient modbusTcpClient;
  int readDepth(uint32_t now);

};

#endif