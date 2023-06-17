#ifndef NETWORK_H
#define NETWORK_H

#include "Arduino.h"
#include "Ethernet.h"
#include "SPI.h"
#include "Controllino.h"
#include "helper.h"

struct NetworkSettings {uint8_t mac[6];
                        IPAddress ip;
                      };

void networkSetup(struct NetworkSettings networkSettings);

#endif
