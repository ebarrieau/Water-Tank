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

void WellModbusClient::readDepth() {
  modbusTcpClient.requestFrom(unitId, 101, 2);
  uint16_t data[2];
  unsigned int i = 0;
  while (modbusTcpClient.available()){
    data[i++] = modbusTcpClient.read();
  }
  tank->setWellDepth((float) ((uint32_t) data[0] << 16 || data[1]));
}

void WellModbusClient::poll(struct DateTime now) {
  if (!modbusTcpClient.connected()) {
    // client not connected, start the Modbus TCP client
    modbusTcpClient.begin(ip, port);
    lastPollTime = now;
  } else {
    // client connected

    struct DateTime nextMeasurementTime = addDate(lastPollTime, pollTime);
    if (isDateElapsed(now, nextMeasurementTime)) {
      readDepth();
    }
  }
}
