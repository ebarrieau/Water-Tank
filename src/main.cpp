//Standard libraries

#include "Arduino.h"

//Third Party Libraries
#include "Controllino.h"

//Private Includes
#include <SimpleTimer.h>
#include <DataStorage.h>
#include <WaterTank.h>
#include <modbusWellClient.h>
#include <ModbusServer.h>
#include <network.h>

volatile int16_t currentSeconds = 0;

DataStorage::WaterTankData data;
DataStorage::WaterTankSettings settings;

settings.tankFillSolenoidPin = CONTROLLINO_R2;
settings.wellPumpContactor = CONTROLLINO_R0;
settings.tankScaleAnalogPin =  CONTROLLINO_AI12
settings.wellPumpContactorPin = CONTROLLINO_R1;
settings.wellIP = IPAddress(192,168,1,202);

void setup()
{

  Serial.begin(115200);
  while (!Serial) {
    ; //wait for serial port to connect
  }

  struct NetworkSettings networkSettings = {{0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED},
                                            IPAddress(192,168,1,201),
                                            CONTROLLINO_ETHERNET_CHIP_SELECT};
  int networkSetupStatus = networkSetup(networkSettings);

  SimpleTimer::Setup(&currentSeconds);
  WellClient::setup(settings);
  int serverSetupStatus = ModbusTankServer::setup(502, settings);

  if (!networkSetupStatus || !serverSetupStatus) {
    while(1) {
      // Delay forever, fatal setup error
    }
  }

}


void loop()
{

  WellClient::poll(settings, data, currentSeconds);

  WaterTank::stopTankIfNeeded(settings, data, currentSeconds);
  WaterTank::startTankIfNeeded(settings, data, currentSeconds);
  WaterTank::manageHousePump(settings, data);
  WaterTank::manageWellPump(settings, data, currentSeconds);

  ModbusTankServer::poll(settings, data, currentSeconds);

}

