// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "user_system_file_provider_impl.h"

#include <stdbool.h>

#include "immutable_registry.h"
#include "registry_api.h"

enum {
  kNonsenseRegistryKey = -1,
  kNonsenseFileAddress = 0,
};

static const int kRegistryKeysForSystemFileAddress[] = {
    kNonsenseRegistryKey,          kUrkRecoverySystemFileAddress,
    kUrkUpdateSystemFileAddress,   kUrkNormalSystemFileAddress,
    kUrkAdvancedSystemFileAddress, kNonsenseRegistryKey,
    kNonsenseRegistryKey,
};

static bool IsInvalid(int runlevel) {
  return (runlevel < kLowestRunlevel) || (runlevel > kHighestRunlevel);
}

static uintptr_t getFileAddress(int runlevel) {
  if (IsInvalid(runlevel)) return kNonsenseFileAddress;

  int key = kRegistryKeysForSystemFileAddress[runlevel];
  RegistryApi ra = RegistryApi_getInstance();

  return ra->getUintptrRegistryValue(key);
}

static IUserSystemFileProviderStruct its_instance = {
    .getFileAddress = getFileAddress,
};
IUserSystemFileProvider UserSystemFileProviderImpl_getInstance(void) {
  return &its_instance;
}
