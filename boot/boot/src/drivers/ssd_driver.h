// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_DRIVERS_SSD_DRIVER_H_
#define BOOT_BOOT_SRC_DRIVERS_SSD_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct SsdDriverStruct* SsdDriver;
typedef uint8_t (*ssdDecoder)(char);

SsdDriver SsdDriver_Create(uint8_t* io_address, ssdDecoder decoder);
void SsdDriver_Destroy(SsdDriver* self);
void SsdDriver_TurnOn(SsdDriver self, int segment_number);
void SsdDriver_TurnOff(SsdDriver self, int segment_number);
void SsdDriver_TurnAllOn(SsdDriver self);
void SsdDriver_TurnAllOff(SsdDriver self);
bool SsdDriver_IsOn(SsdDriver self, int segment_number);
bool SsdDriver_IsOff(SsdDriver self, int segment_number);
void SsdDriver_Set(SsdDriver self, uint8_t encoding);
uint8_t SsdDriver_Get(SsdDriver self);

#endif  // BOOT_BOOT_SRC_DRIVERS_SSD_DRIVER_H_
