#include "network.h"

void networkSetup(struct NetworkSettings networkSettings) {
  Ethernet.init(CONTROLLINO_ETHERNET_CHIP_SELECT);
  Ethernet.begin(networkSettings.mac, networkSettings.ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    delayForever();
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
}
