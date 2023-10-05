#include "network.h"

int networkSetup(struct NetworkSettings networkSettings) {
  Ethernet.init(networkSettings.chipSelectPin);
  Ethernet.begin(networkSettings.mac, networkSettings.ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    // Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    return 0;
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    // Serial.println("Ethernet cable is not connected.");
  }
  return 1;
}
