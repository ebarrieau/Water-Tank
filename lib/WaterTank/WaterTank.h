#ifndef WATER_TANK_H
#define WATER_TANK_H

#include <Arduino.h>
#include <DataStorage.h>
#include <SimpleTimer.h>

namespace WaterTank
{
    bool tankIsFilling(DataStorage::WaterTankSettings &settings);
    bool tankIsOverfull(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    bool tankIsFull(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    bool tankExceedsFillingTarget(DataStorage::WaterTankData &data);
    bool tankIsNotFull(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    bool tankIsEmpty(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    bool tankIsNotEmpty(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    bool wellIsAtWorkingDepth(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    bool wellIsAtSafetyDepth(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    bool tankFillingTimeoutExceeded(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now);
    bool wellRechargeComplete(DataStorage::WaterTankData &data, uint16_t now);
    bool wellDepthDataIsCurrent(DataStorage::WaterTankData &data, uint16_t now);
    bool housePumpIsRunning(DataStorage::WaterTankSettings &settings);
    bool wellPumpIsRunning(DataStorage::WaterTankSettings &settings);
    uint16_t calculateWeightTarget(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    uint8_t stopTankIfNeeded(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now);
    uint8_t startTankIfNeeded(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now);
    uint8_t manageHousePump(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    uint8_t manageWellPump(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now);

}

#endif