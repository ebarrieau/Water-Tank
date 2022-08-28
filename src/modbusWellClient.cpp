#include "modbusWellClient.h"

WellModbusClient::WellModbusClient(Controller *tank,
                                   uint8_t unitId,
                                   IPAddress ip,
                                   uint16_t port) : tank{tank},
                                                    unitId{unitId},
                                                    ip{ip},
                                                    port{port},
                                                    ethClient(port),
                                                    modbusTcpClient(ethClient)
{

}

int WellModbusClient::readDepth(uint32_t now) {
  if(!modbusTcpClient.requestFrom(unitId, HOLDING_REGISTERS, 101, 2)){
    // Serial.print("Read Depth Failed ");
    // Serial.println(modbusTcpClient.lastError());
    return 0;
  }
  uint16_t data[2];
  unsigned int i = 0;
  while (modbusTcpClient.available()){
    data[i++] = modbusTcpClient.read();
  }
  uint32_t depthInt = (uint32_t) data[1] << 16 | data[0];
  float depthFloat;
  memcpy(&depthFloat, &depthInt, sizeof(depthFloat));
  depthFloat = depthFloat * 10;

  tank->setWellDepth((uint16_t) depthFloat);
  tank->setWellGoodTime(now);
  return 1;
}

void WellModbusClient::poll(uint32_t now) {
  if (!modbusTcpClient.connected()) {
    // client not connected, start the Modbus TCP client
    modbusTcpClient.begin(ip, port);
    lastPollTime = now;
  } else {
    // client connected
    uint32_t nextMeasurementTime = addDate(lastPollTime, pollTime, false);

    if (isDateElapsed(now, nextMeasurementTime)) {
      if( readDepth(now) ){
        lastPollTime = now;
      }
    }
  }
}
