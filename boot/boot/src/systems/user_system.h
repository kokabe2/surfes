// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_SYSTEMS_USER_SYSTEM_H_
#define BOOT_BOOT_SRC_SYSTEMS_USER_SYSTEM_H_

#include <stdint.h>

#include "script/system_executable.h"

void UserSystem_Change(uintptr_t file_address);
ISystemExecutable UserSystem_getInstance(void);

#endif  // BOOT_BOOT_SRC_SYSTEMS_USER_SYSTEM_H_
