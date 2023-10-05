#ifndef TANK_MODBUS_SERVER_H
#define TANK_MODBUS_SERVER_H

#include "Arduino.h"
#include "Ethernet.h"
#include "ArduinoRS485.h"
#include "ArduinoModbus.h"
#include "DataStorage.h"

namespace ModbusTankServer
{
    int setup(uint16_t port, DataStorage::WaterTankSettings &settings);
    void poll(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    void updateInputs(ModbusTCPServer &server, DataStorage::WaterTankData &data);
}

#endif