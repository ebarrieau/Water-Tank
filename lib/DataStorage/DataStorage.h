#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <Arduino.h>

namespace DataStorage
{
  struct WaterTankSettings 
  {
    uint16_t maxWeight = 35000;
    uint16_t maxIncWeight = 3000;
    uint16_t hysteresis = 500;
    uint16_t wellWorkingDepth = 1500;
    uint16_t wellSafetyDepth = 2000;
    uint16_t wellRechargeMinTime = 3600;
    uint16_t wellRechargeCompleteDepth = 750;
    uint16_t tankMinHousePumpWeight = 5000;
    unsigned int tankFillSolenoidPin;
    unsigned int housePumpContactorPin;
    unsigned int tankScaleAnalogPin;
    unsigned int wellPumpContactorPin;
  };

  struct WaterTankData
  {
    uint16_t weight;
    uint16_t wellDepth;
    uint16_t fillingWeightTarget;
    int16_t fillingTimeTarget;
    int16_t rechargeTimeTarget;
    int16_t wellDataGoodUntil;
  };
}

#endif