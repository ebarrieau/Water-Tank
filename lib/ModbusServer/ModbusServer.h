#ifndef MODBUS_SERVER_H
#define MODBUS_SERVER_H

#include "Arduino.h"
#include "Ethernet.h"
#include "ArduinoRS485.h"
#include "ArduinoModbus.h"
#include "DataStorage.h"

namespace TankServer
{
    int setup(uint16_t port, DataStorage::WaterTankSettings &settings);
    void poll(ModbusTCPServer &server, DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    void updateInputs(ModbusTCPServer &server, DataStorage::WaterTankData &data);
}

#endif