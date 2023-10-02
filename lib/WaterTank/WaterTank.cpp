#include <WaterTank.h>

bool WaterTank::tankIsFilling(DataStorage::WaterTankSettings &settings)
{
    return digitalRead(settings.tankFillSolenoidPin);
}

bool WaterTank::tankIsOverfull(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data)
{
    return (data.weight >= (settings.maxWeight + settings.hysteresis));
}

bool WaterTank::tankIsFull(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data)
{
    return (data.weight >= settings.maxWeight);
}

bool WaterTank::tankExceedsFillingTarget(DataStorage::WaterTankData &data)
{
    return (data.weight >= data.fillingWeightTarget);
}

bool WaterTank::tankIsNotFull(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data)
{
    return (data.weight < (settings.maxWeight - settings.maxIncWeight));
}

bool WaterTank::tankIsEmpty(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data)
{
    return (data.weight <= settings.tankMinHousePumpWeight);
}

bool WaterTank::tankIsNotEmpty(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data)
{
    return (data.weight > (settings.tankMinHousePumpWeight + settings.hysteresis));
}

bool WaterTank::wellIsAtWorkingDepth(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data)
{
    return (data.wellDepth >= settings.wellWorkingDepth);
}

bool WaterTank::wellIsAtSafetyDepth(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data)
{
    return (data.wellDepth >= settings.wellSafetyDepth);
}

bool WaterTank::tankFillingTimeoutExceeded(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now)
{
    return (WaterTank::tankIsFilling(settings) && SimpleTimer::Ended(now, data.fillingTimeTarget));
}

bool WaterTank::wellRechargeComplete(DataStorage::WaterTankData &data, uint16_t now)
{
    return SimpleTimer::Ended(now, data.rechargeTimeTarget);
}

bool WaterTank::wellDepthDataIsCurrent(DataStorage::WaterTankData &data, uint16_t now)
{
    return !SimpleTimer::Ended(now, data.wellDataGoodUntil);
}

bool WaterTank::housePumpIsRunning(DataStorage::WaterTankSettings &settings)
{
    return digitalRead(settings.housePumpContactorPin);
}

bool WaterTank::wellPumpIsRunning(DataStorage::WaterTankSettings &settings)
{
    return !digitalRead(settings.wellPumpContactorPin);
}

void WaterTank::updateWeight(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data)
{
  int loadCellRaw = analogRead(settings.tankScaleAnalogPin);
  //500ohm Resistor 24V supply - 4-20mA = 2-10V = 204.8-1023 steps
  data.weight = map(loadCellRaw, 205, 1023, 0, 50000);
}

uint16_t WaterTank::calculateWeightTarget(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data)
{
    uint16_t spaceInTank = settings.maxWeight - data.weight;
    uint16_t availableInWell = (settings.wellWorkingDepth - data.wellDepth) * 1.4687965402597 * 8.34; //depth in feet * gallons per foot * pounds per gallon
    //Depth and weight are both fixed point numbers with 1 decimal place 500 -> 50.0
    uint16_t target = min(spaceInTank, availableInWell);
    target = min(target, settings.maxIncWeight);
    return target;
}

uint8_t WaterTank::stopTankIfNeeded(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now)
{
    uint8_t results = 0;
    
    results |= WaterTank::tankIsFull(settings, data);
    results |= WaterTank::wellIsAtWorkingDepth(settings, data) << 1;
    results |= WaterTank::tankFillingTimeoutExceeded(settings, data, now) << 2;
    results |= WaterTank::tankExceedsFillingTarget(data) << 3;


    if (results != 0 && WaterTank::tankIsFilling(settings))
    {
        digitalWrite(settings.tankFillSolenoidPin, LOW);
        data.rechargeTimeTarget = SimpleTimer::Start(now, settings.wellRechargeMinTime);
        return 1;
    }

    return 0;
}

uint8_t WaterTank::startTankIfNeeded(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now)
{
    uint8_t results = 0;
    results |= WaterTank::tankIsFilling(settings);
    results |= WaterTank::tankIsNotFull(settings, data) << 1;
    results |= WaterTank::wellIsAtWorkingDepth(settings, data) << 2;
    results |= WaterTank::wellRechargeComplete(data, now) << 3;
    results |= WaterTank::wellDepthDataIsCurrent(data, now) << 4;

    if (results == 0b00011010)
    {
        uint16_t currentIncWeightTarget = WaterTank::calculateWeightTarget(settings, data);
        data.fillingWeightTarget = data.weight + currentIncWeightTarget;
        uint16_t expectedFillTimeSeconds = currentIncWeightTarget / 6.95; // target lbs / ( 5 gal/min / 60s/min * lbs/gal * 10 Fixed point offset)
        data.fillingTimeTarget = SimpleTimer::Start(now, expectedFillTimeSeconds * 1.2);
        
        digitalWrite(settings.tankFillSolenoidPin, HIGH);
        return 1;
    }

    return 0;
}

uint8_t WaterTank::manageHousePump(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data)
{
    bool pumpIsRunning = WaterTank::housePumpIsRunning(settings);
    bool tankIsEmpty = WaterTank::tankIsEmpty(settings, data);
    bool tankIsNotEmpty = WaterTank::tankIsNotEmpty(settings, data);

    if (pumpIsRunning && tankIsEmpty)
    {
        digitalWrite(settings.housePumpContactorPin, LOW);
        return 0;
    } else if (!pumpIsRunning && tankIsNotEmpty)
    {
        digitalWrite(settings.housePumpContactorPin, HIGH);
        return 1;
    }

    return pumpIsRunning;
}

uint8_t WaterTank::manageWellPump(DataStorage::WaterTankSettings &settings, DataStorage::WaterTankData &data, uint16_t now)
{
    bool pumpIsRunning = WaterTank::wellPumpIsRunning(settings);
    bool wellDataIsCurrent = WaterTank::wellDepthDataIsCurrent(data, now);
    uint8_t results = 0;
    results |= WaterTank::tankIsOverfull(settings, data) ;
    results |= WaterTank::wellIsAtSafetyDepth(settings, data) << 1;


    if (results != 0 && pumpIsRunning)
    {
        //Well contactor is Normally Closed, so HIGH output logic turns the pump off
        digitalWrite(settings.wellPumpContactorPin, HIGH);
        return 0;
    } else if (results == 0 && wellDataIsCurrent && !pumpIsRunning)
    {
        digitalWrite(settings.wellPumpContactorPin, LOW);
        return 1;
    }

    return pumpIsRunning;
}