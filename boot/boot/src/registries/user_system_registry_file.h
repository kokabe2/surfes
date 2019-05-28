// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_REGISTRIES_USER_SYSTEM_REGISTRY_FILE_H_
#define BOOT_BOOT_SRC_REGISTRIES_USER_SYSTEM_REGISTRY_FILE_H_

#include <stdint.h>

#include "script/system_executable.h"

typedef struct UserSystemRegistryFileStruct *UserSystemRegistryFile;

UserSystemRegistryFile UserSystemRegistryFile_Open(uintptr_t address);
void UserSystemRegistryFile_Close(UserSystemRegistryFile *self);
ISystemExecutable UserSystemRegistryFile_getExecutor(
    UserSystemRegistryFile self, int runlevel);

#endif  // BOOT_BOOT_SRC_REGISTRIES_USER_SYSTEM_REGISTRY_FILE_H_
