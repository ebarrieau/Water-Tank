#ifndef NETWORK_H
#define NETWORK_H

#include "Arduino.h"
#include "Ethernet.h"
#include "SPI.h"

struct NetworkSettings {uint8_t mac[6];
                        IPAddress ip;
                        uint8_t chipSelectPin;
                      };

int networkSetup(struct NetworkSettings networkSettings);

#endif
