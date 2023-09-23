#ifndef MODBUS_WELL_CLIENT_H
#define MODBUS_WELL_CLIENT_H

#include "Arduino.h"
#include "Ethernet.h"
#include "ArduinoRS485.h"
#include "ArduinoModbus.h"
#include "DataStorage.h"
#include "SimpleTimer.h"

namespace WellClient
{
    void setup(DataStorage::WaterTankSettings &settings);
    int poll(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now);
    int readDepth(DataStorage::WaterTankData &data);
}

#endif