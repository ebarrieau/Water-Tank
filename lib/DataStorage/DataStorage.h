#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <Arduino.h>
#include <Ethernet.h>
#include <ArduinoModbus.h>

namespace DataStorage
{
  struct WaterTankSettings 
  {
    uint16_t maxWeight;
    uint16_t maxIncWeight;
    uint16_t hysteresis;
    uint16_t wellWorkingDepth;
    uint16_t wellSafetyDepth;
    uint16_t wellRechargeMinTime;
    uint16_t tankSolenoidTimeout;
    uint16_t wellRechargeCompleteDepth;
    uint16_t tankMinHousePumpWeight;
    unsigned int tankFillSolenoidPin;
    unsigned int housePumpContactorPin;
    unsigned int tankScaleAnalogPin;
    unsigned int wellPumpContactorPin;
    IPAddress wellIP;
    ModbusTCPClient wellClient;
    uint16_t port;
    uint16_t wellPollTimeSetpoint;
    uint16_t wellDataGoodUntilSetpoint;
    EthernetServer ethServer;
    EthernetClient ethClient;
    ModbusTCPServer server;

    
  };

  struct WaterTankData
  {
    uint16_t weight;
    uint16_t wellDepth;
    uint16_t fillingWeightTarget;
    int16_t fillingTimeTarget;
    int16_t rechargeTimeTarget;
    int16_t wellDataGoodUntil;
    int16_t wellPollTimeTarget;
    int16_t tankSolenoidTimeoutTarget;
    bool wellRechargeComplete;
  };
}

#endif