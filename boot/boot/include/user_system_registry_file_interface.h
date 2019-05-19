// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_INCLUDE_USER_SYSTEM_REGISTRY_FILE_INTERFACE_H_
#define BOOT_BOOT_INCLUDE_USER_SYSTEM_REGISTRY_FILE_INTERFACE_H_

#include "script/system_executable.h"

enum {
  kLowestRunlevel = 0,
  kHighestRunlevel = 6,
  kNumOfRunlevels = 7,
  kUserSystemRegistryFileAddress = 0xFFC00000,
};

typedef struct UserSystemRegistryFileEntryPointStruct {
  ISystemExecutable (*getExecutor)(int runlevel);
} UserSystemRegistryFileEntryPointStruct, *UserSystemRegistryFileEntryPoint;

#endif  // BOOT_BOOT_INCLUDE_USER_SYSTEM_REGISTRY_FILE_INTERFACE_H_
