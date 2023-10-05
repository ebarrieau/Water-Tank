#include "TankModbusServer.h"

int ModbusTankServer::setup(uint16_t port, DataStorage::WaterTankSettings &settings) {
    // start the server
    settings.ethServer.begin();


    // start the Modbus TCP server
    if (!settings.server.begin()) {
        // Serial.println("Failed to start Modbus TCP Server!");
        return 0;
    }

    settings.server.configureInputRegisters(0x00, 6);

    return 1;
}

void ModbusTankServer::updateInputs(ModbusTCPServer &server, DataStorage::WaterTankData &data)
{
  server.inputRegisterWrite(0x00, data.weight);
  server.inputRegisterWrite(0x01, data.wellDepth);
  server.inputRegisterWrite(0x02, data.fillingWeightTarget);
  server.inputRegisterWrite(0x03, data.wellDataGoodUntil);

}

void ModbusTankServer::poll(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data)
{
  ModbusTankServer::updateInputs(settings.server, data);

  if (!settings.ethClient || !settings.ethClient.connected()) {

    EthernetClient client = settings.ethServer.available();
    if (client) {
      settings.ethClient = client;
      settings.server.accept(settings.ethClient);
    }
  }

  if (settings.ethClient.connected()) {
    settings.server.poll();
  }
}
