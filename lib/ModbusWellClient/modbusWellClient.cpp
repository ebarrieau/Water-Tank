#include "ModbusWellClient.h"

void WellClient::setup(DataStorage::WaterTankSettings &settings)
{
    EthernetClient ethClient;
    ModbusTCPClient modbusClient(ethClient);
    settings.wellClient = modbusClient;
}

int WellClient::poll(ModbusTCPClient &client, DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now)
{
    if (!client.connected()) {
    // client not connected, start the Modbus TCP client
        client.begin(settings.wellIP, settings.port);
        data.wellPollTimeTarget = now;
    } else {
    // client connected
        if (SimpleTimer::Ended(now, data.wellPollTimeTarget)) {
            if(WellClient::readDepth(client, data)){
                data.wellPollTimeTarget = SimpleTimer::Start(now, settings.wellPollTimeSetpoint);
                data.wellDataGoodUntil = SimpleTimer::Start(now, settings.wellDataGoodUntilSetpoint);
                return 1;
            } else {
                client.stop();
            }
        }
    }
    return 0;
}

int WellClient::readDepth(ModbusTCPClient &client, DataStorage::WaterTankData &data)
{
    if(!client.requestFrom(11, HOLDING_REGISTERS, 101, 2)){
        return 0;
    }
    uint16_t buffer[2];
    unsigned int i = 0;
    while (client.available()){
        buffer[i++] = client.read();
    }
    uint32_t depthInt = (uint32_t) buffer[1] << 16 | buffer[0];
    float depthFloat;
    memcpy(&depthFloat, &depthInt, sizeof(depthFloat));
    depthFloat = depthFloat * 10;

    data.wellDepth = (uint16_t) depthFloat;
    return 1;
}