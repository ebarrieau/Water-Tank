//Standard libraries

#include "Arduino.h"
#include "Ethernet.h"

//Third Party Libraries
#include "Controllino.h"
#include "MemoryUsage.h"

//Private 
#include <TankModbusServer.h>
#include <SimpleTimer.h>
#include <DataStorage.h>
#include <WaterTank.h>
#include <modbusWellClient.h>
#include <network.h>


volatile int16_t currentSeconds = 0;

// IPAddress* wellIP = new IPAddress(192,168,1,202);

DataStorage::WaterTankData data = {0,0,0,0,0,0,0,0,true};
DataStorage::WaterTankSettings settings = {
  40000, //maxWeight
  3000, //maxIncWeight
  500, //hysteresis
  1750, //wellWorkingDepth
  2000, //wellSafetyDepth
  1800, //wellRechargeMinTime
  300, //tankSolenoidTimeout
  1200, //wellRechargeCompleteDepth
  5000, //tankMinHousePumpWeight
  CONTROLLINO_R2, //tankFillSolenoidPin
  CONTROLLINO_R0, //housePumpContactorPin
  CONTROLLINO_AI12, //tankScaleAnalogPin
  CONTROLLINO_R1, //wellPumpContactorPin
  IPAddress(192,168,1,202), //wellIP
  WellClient::setup(), //wellClient
  502, //well Port
  1, //wellPollTimeSetpoint
  600, //wellDataGoodUntilSetpoint
  EthernetServer(502), //ethServer
  EthernetClient(), //ethClient
  ModbusTCPServer() //server
};

void setup()
{

  // Serial.begin(115200);
  // while (!Serial) {
  //   ; //wait for serial port to connect
  // }

  pinMode(settings.tankFillSolenoidPin, OUTPUT);
  pinMode(settings.housePumpContactorPin, OUTPUT);
  pinMode(settings.wellPumpContactorPin, OUTPUT);

  struct NetworkSettings networkSettings = {{0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED},
                                            IPAddress(192,168,1,201),
                                            CONTROLLINO_ETHERNET_CHIP_SELECT};
  int networkSetupStatus = networkSetup(networkSettings);

  SimpleTimer::Setup(&currentSeconds);
  int serverSetupStatus = ModbusTankServer::setup(502, settings);

  if (!networkSetupStatus || !serverSetupStatus) {
    while(1) {
      // Delay forever, fatal setup error
    }
  }
}


void loop()
{
  // FREERAM_PRINT;

  WellClient::poll(settings, data, currentSeconds);

  WaterTank::updateWeight(settings, data);
  WaterTank::updateWellRechargeStatus(settings, data, currentSeconds);

  WaterTank::stopTankIfNeeded(settings, data, currentSeconds);
  WaterTank::startTankIfNeeded(settings, data, currentSeconds);
  WaterTank::manageHousePump(settings, data);
  WaterTank::manageWellPump(settings, data, currentSeconds);

  ModbusTankServer::poll(settings, data);

}

