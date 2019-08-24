// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_UTILITY_INC_SSD_DRIVER_H_
#define DEVELOPMENTS_UTILITY_INC_SSD_DRIVER_H_

#include "led_driver.h"

typedef uint8_t (*ssdDecoder)(char);

LedDriver SsdDriver_Create(uint8_t* io_address, ssdDecoder decoder);
void SsdDriver_Set(LedDriver self, char encoding);
char SsdDriver_Get(LedDriver self);

#endif  // DEVELOPMENTS_UTILITY_INC_SSD_DRIVER_H_
