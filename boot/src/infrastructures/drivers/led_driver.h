// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_SRC_INFRASTRUCTURES_DRIVERS_LED_DRIVER_H_
#define BOOT_SRC_INFRASTRUCTURES_DRIVERS_LED_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct LedDriverStruct* LedDriver;

LedDriver LedDriver_Create(uint8_t* io_address, uint8_t (*decoder)(int));
void LedDriver_Destroy(LedDriver* self);
void LedDriver_TurnOn(LedDriver self, int led_number);
void LedDriver_TurnOff(LedDriver self, int led_number);
void LedDriver_TurnAllOn(LedDriver self);
void LedDriver_TurnAllOff(LedDriver self);
bool LedDriver_IsOn(LedDriver self, int led_number);
bool LedDriver_IsOff(LedDriver self, int led_number);

#endif  // BOOT_SRC_INFRASTRUCTURES_DRIVERS_LED_DRIVER_H_
