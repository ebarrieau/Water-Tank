#ifndef MODBUS_WELL_CLIENT_H
#define MODBUS_WELL_CLIENT_H

#include "Arduino.h"
#include "Ethernet.h"
#include "ArduinoRS485.h"
#include "ArduinoModbus.h"

namespace WaterTank
{
    ModbusTCPClient setup(IPAddress ip, uint16_t port);
    

}

#endif