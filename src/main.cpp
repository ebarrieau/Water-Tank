//Standard libraries

#include "Arduino.h"

//Third Party Libraries


//Private Includes
#include "SimpleTimer.h"
#include "DataStorage.h"
#include "WaterTank.h"
#include "modbusWellClient.h"
#include "ModbusServer.h"

volatile int16_t currentSeconds = 0;

DataStorage::WaterTankData data;
DataStorage::WaterTankSettings settings;

void setup()
{

  Serial.begin(115200);
  while (!Serial) {
    ; //wait for serial port to connect
  }

  SimpleTimer::Setup(&currentSeconds);
  WellClient::setup(settings);
  TankServer::setup(settings);

}


void loop()
{

  WellClient::poll(settings, data, currentSeconds);

  WaterTank::stopTankIfNeeded(settings, data, currentSeconds);
  WaterTank::startTankIfNeeded(settings, data, currentSeconds);
  WaterTank::manageHousePump(settings, data);
  WaterTank::manageWellPump(settings, data, currentSeconds);

  TankServer::poll(settings, data, currentSeconds);

}

