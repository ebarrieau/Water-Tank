#ifndef MODBUS_WELL_CLIENT_H
#define MODBUS_WELL_CLIENT_H

#include "Arduino.h"
#include "Ethernet.h"
#include "ArduinoRS485.h"
#include "ArduinoModbus.h"
#include "DataStorage.h"
#include "SimpleTimer.h"

namespace WaterTank
{
    ModbusTCPClient setup();
    int poll(ModbusTCPClient &client, DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now);
    int readDepth(ModbusTCPClient &client, DataStorage::WaterTankData &data);
}

#endif