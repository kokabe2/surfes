// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_REGISTRY_INCLUDE_REGISTRY_FILE_API_H_
#define BOOT_REGISTRY_INCLUDE_REGISTRY_FILE_API_H_

#include <stdint.h>

enum RegistryFileApiError {
  kRfaNoError = 0,
  kRfaNoRegistryKeyError,
};

enum IntRegistryKey {
  kIrkDefaultRunlevel = 0,
  kIrkLowestRunlevel,
  kIrkHighestRunlevel,
  kIrkNumOfRunlevels,
  kIrkHaltModeRunlevel,
  kIrkRecoveryModeRunlevel,
  kIrkUpdateModeRunlevel,
  kIrkUserModeRunlevel,
  kIrkDeveloperModeRunlevel,
  kIrkRebootModeRunlevel,
};

enum UintptrRegistryKey {
  kUrkCoreFileAddress = 0,
  kUrkUpdaterFileAddress,
};

typedef struct RegistryFileEntryPointStruct {
  int (*getIntRegistryValue)(int key);
  uintptr_t (*getUintptrRegistryValue)(int key);
} RegistryFileEntryPointStruct, *RegistryFileEntryPoint;

#endif  // BOOT_REGISTRY_INCLUDE_REGISTRY_FILE_API_H_
