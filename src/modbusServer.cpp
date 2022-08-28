#include "modbusServer.h"

TankModbusServer::TankModbusServer(Controller *tank, uint16_t port) : tank{tank},
                                                                      ethServer(port)
{

}

void TankModbusServer::setup() {
  // start the server
  ethServer.begin();

  // start the Modbus TCP server
  if (!modbusTCPServer.begin()) {
    // Serial.println("Failed to start Modbus TCP Server!");
    delayForever();
  }

  modbusTCPServer.configureCoils(0x00, 2);
  modbusTCPServer.configureDiscreteInputs(0x00, 2);
  modbusTCPServer.configureInputRegisters(0x00, 5);
  modbusTCPServer.configureHoldingRegisters(0x00, 6);
}

void TankModbusServer::updateInputs() {
  modbusTCPServer.discreteInputWrite(0x00, tank->getPumpState());
  modbusTCPServer.discreteInputWrite(0x01, tank->getSolenoidState());
  modbusTCPServer.inputRegisterWrite(0x00, tank->getWeight());

  // float depthFloat = tank->getWellDepth();
  // uint32_t depthInt;
  // memcpy(&depthInt, &depthFloat, sizeof(depthInt));
  // uint16_t highWord = depthInt >> 16;
  // uint16_t lowWord = depthInt & 0xFFFF;

  modbusTCPServer.inputRegisterWrite(0x01, tank->getWellDepth());
  modbusTCPServer.inputRegisterWrite(0x02, tank->getTargetWeight());
  uint16_t lowWord = tank->getWellGoodTime() >> 16;
  uint16_t highWord = tank->getWellGoodTime() & 0xFFFF;
  modbusTCPServer.inputRegisterWrite(0x03, highWord);
  modbusTCPServer.inputRegisterWrite(0x04, lowWord);
}

void TankModbusServer::poll() {
  updateInputs();

  if (!client || !client.connected()) {
    client = ethServer.available();
    if (client) {
      modbusTCPServer.accept(client);
    }
  }

  if (client.connected()) {
    modbusTCPServer.poll();
  }
}
