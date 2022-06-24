#include "modbusServer.h"

TankModbusServer::TankModbusServer(Controller tank,
                                   uint8_t mac[6],
                                   uint8_t *address) : tank{tank},
                                                       mac{mac},
                                                       ip(address),
                                                       ethServer(502)
{

}

void TankModbusServer::setup() {
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
