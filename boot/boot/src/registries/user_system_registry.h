// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_REGISTRIES_USER_SYSTEM_REGISTRY_H_
#define BOOT_BOOT_SRC_REGISTRIES_USER_SYSTEM_REGISTRY_H_

#include <stdint.h>

#include "system_executable.h"

void UserSystemRegistry_Change(uintptr_t address);
ISystemExecutable UserSystemRegistry_getExecutor(int runlevel);
int UserSystemRegistry_getDefaultRunlevel(void);

#endif  // BOOT_BOOT_SRC_REGISTRIES_USER_SYSTEM_REGISTRY_H_
