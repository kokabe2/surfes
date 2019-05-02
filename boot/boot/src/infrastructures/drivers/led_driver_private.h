// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_INFRASTRUCTURES_DRIVERS_LED_DRIVER_PRIVATE_H_
#define BOOT_BOOT_SRC_INFRASTRUCTURES_DRIVERS_LED_DRIVER_PRIVATE_H_

#include <stdint.h>

typedef struct LedDriverStruct {
  uint8_t* io_address;
  uint8_t (*decoder)(int);
} LedDriverStruct;

#endif  // BOOT_BOOT_SRC_INFRASTRUCTURES_DRIVERS_LED_DRIVER_PRIVATE_H_
