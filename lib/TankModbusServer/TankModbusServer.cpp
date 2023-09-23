#include "TankModbusServer.h"

int ModbusTankServer::setup(uint16_t port, DataStorage::WaterTankSettings &settings) {
    // start the server
    settings.ethServer = new EthernetServer(port);
    settings.ethServer->begin();

    settings.server = new ModbusTCPServer;

    // start the Modbus TCP server
    if (!settings.server->begin()) {
        // Serial.println("Failed to start Modbus TCP Server!");
        return 0;
    }

    settings.server->configureInputRegisters(0x00, 6);

    return 1;
}

void ModbusTankServer::updateInputs(ModbusTCPServer &server, DataStorage::WaterTankData &data)
{
  server.inputRegisterWrite(0x00, data.weight);
  server.inputRegisterWrite(0x01, data.wellDepth);
  server.inputRegisterWrite(0x02, data.fillingWeightTarget);
  uint16_t lowWord = data.wellDataGoodUntil >> 16;
  uint16_t highWord = data.wellDataGoodUntil & 0xFFFF;
  server.inputRegisterWrite(0x03, highWord);
  server.inputRegisterWrite(0x04, lowWord);

}

void ModbusTankServer::poll(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data)
{
  ModbusTankServer::updateInputs(*settings.server, data);

  if (!settings.ethClient || !settings.ethClient->connected()) {
    if (settings.ethClient)
    {
      delete settings.ethClient;
    }
    EthernetClient client = settings.ethServer->available();
    if (client) {
      settings.ethClient = new EthernetClient();
      memcpy(settings.ethClient, &client, sizeof(client));
      settings.server->accept(*settings.ethClient);
    }
  }

  if (settings.ethClient->connected()) {
    settings.server->poll();
  }
}
