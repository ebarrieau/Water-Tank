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
    void updateWellRechargeStatus(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now);
    bool wellRechargeCompleteTime(DataStorage::WaterTankData &data, uint16_t now);
    bool wellRechargeCompleteDepth(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    bool wellDepthDataIsCurrent(DataStorage::WaterTankData &data, uint16_t now);
    bool housePumpIsRunning(DataStorage::WaterTankSettings &settings);
    bool wellPumpIsRunning(DataStorage::WaterTankSettings &settings);
    bool tankSolenoidTimeoutComplete(DataStorage::WaterTankData &data, uint16_t now);
    uint16_t calculateWeightTarget(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    uint8_t stopTankIfNeeded(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now);
    uint8_t startTankIfNeeded(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now);
    void updateWeight(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    uint8_t manageHousePump(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data);
    uint8_t manageWellPump(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now);

}

#endif