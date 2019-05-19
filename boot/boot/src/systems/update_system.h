// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_SYSTEMS_UPDATE_SYSTEM_H_
#define BOOT_BOOT_SRC_SYSTEMS_UPDATE_SYSTEM_H_

#include <stdint.h>

#include "system_executable.h"

void UpdateSystem_Change(uintptr_t file_address);
ISystemExecutable UpdateSystem_getInstance(void);

#endif  // BOOT_BOOT_SRC_SYSTEMS_UPDATE_SYSTEM_H_
