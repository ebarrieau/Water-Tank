#include "modbusServer.h"

TankModbusServer::TankModbusServer(Controller *tank,
                                   uint8_t mac0,
                                   uint8_t mac1,
                                   uint8_t mac2,
                                   uint8_t mac3,
                                   uint8_t mac4,
                                   uint8_t mac5,
                                   uint8_t address0,
                                   uint8_t address1,
                                   uint8_t address2,
                                   uint8_t address3) : tank{tank},
                                                       mac{mac0, mac1, mac2, mac3, mac4, mac5},
                                                       ip(address0, address1, address2, address3),
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

  modbusTCPServer.configureCoils(0x00, 2);
  modbusTCPServer.configureDiscreteInputs(0x00, 2);
  modbusTCPServer.configureInputRegisters(0x00, 2);
  modbusTCPServer.configureHoldingRegisters(0x00, 6);
}

void TankModbusServer::updateInputs() {
  modbusTCPServer.discreteInputWrite(0x00, tank->getPumpState());
  modbusTCPServer.discreteInputWrite(0x01, tank->getSolenoidState());
  modbusTCPServer.inputRegisterWrite(0x00, tank->getWeight());
}

void TankModbusServer::poll() {
  updateInputs();
  
  if (!client) {
    client = ethServer.available();
    if (client) {
      modbusTCPServer.accept(client);
    }
  }

  if (client.connected()) {
    modbusTCPServer.poll();
  }
}
