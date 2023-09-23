#include "modbusServer.h"

int TankServer::setup(uint16_t port, DataStorage::WaterTankSettings &settings) {
    // start the server
    EthernetServer ethServer(port);
    ethServer.begin();

    settings.ethServer = ethServer;

    ModbusTCPServer server;

    // start the Modbus TCP server
    if (!server.begin()) {
        // Serial.println("Failed to start Modbus TCP Server!");
        return 0;
    }

    server.configureInputRegisters(0x00, 6);

    settings.server = server;
    return 1;
}

void TankServer::updateInputs(ModbusTCPServer &server, DataStorage::WaterTankData &data)
{
  server.inputRegisterWrite(0x01, data.wellDepth);
  server.inputRegisterWrite(0x02, data.fillingWeightTarget);
  uint16_t lowWord = data.wellDataGoodUntil >> 16;
  uint16_t highWord = data.wellDataGoodUntil & 0xFFFF;
  server.inputRegisterWrite(0x03, highWord);
  server.inputRegisterWrite(0x04, lowWord);

}

void TankServer::poll(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data)
{
  TankServer::updateInputs(settings.server, data);

  if (!settings.ethClient || !settings.ethClient.connected()) {
    settings.ethClient = settings.ethServer.available();
    if (settings.ethClient) {
      settings.server.accept(settings.ethClient);
    }
  }

  if (settings.ethClient.connected()) {
    settings.server.poll();
  }
}
