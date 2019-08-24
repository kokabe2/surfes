// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_UTILITY_SRC_INFRASTRUCTURES_LED_DRIVER_PRIVATE_H_
#define DEVELOPMENTS_UTILITY_SRC_INFRASTRUCTURES_LED_DRIVER_PRIVATE_H_

#include <stdint.h>

typedef struct LedDriverStruct {
  uint8_t* io_address;
  uint8_t (*decoder)(int);
} LedDriverStruct;

#endif  // DEVELOPMENTS_UTILITY_SRC_INFRASTRUCTURES_LED_DRIVER_PRIVATE_H_
