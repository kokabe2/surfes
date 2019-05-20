// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef UTILITY_INCLUDE_LED_DRIVER_H_
#define UTILITY_INCLUDE_LED_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct LedDriverStruct* LedDriver;
typedef uint8_t (*ledDecoder)(int);

LedDriver LedDriver_Create(uint8_t* io_address, ledDecoder decoder);
void LedDriver_Destroy(LedDriver* self);
void LedDriver_TurnOn(LedDriver self, int led_number);
void LedDriver_TurnOff(LedDriver self, int led_number);
void LedDriver_TurnAllOn(LedDriver self);
void LedDriver_TurnAllOff(LedDriver self);
bool LedDriver_IsOn(LedDriver self, int led_number);
bool LedDriver_IsOff(LedDriver self, int led_number);

#endif  // UTILITY_INCLUDE_LED_DRIVER_H_
