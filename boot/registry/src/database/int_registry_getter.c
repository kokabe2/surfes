// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "int_registry_getter.h"

#include <stdbool.h>

#include "registry_default_api.h"
#include "runtime_error.h"

enum {
  kNonsenseValue = -1,
};

static const int kIntRegistryValues[] = {
    kDefaultRunlevel,    kLowestRunlevel,   kHighestRunlevel,
    kNumOfRunlevels,     kHaltModeRunlevel, kRecoveryModeRunlevel,
    kUpdateModeRunlevel, kUserModeRunlevel, kDeveloperModeRunlevel,
    kRebootModeRunlevel,
};

static bool IsInvalid(int key) {
  int limited = sizeof(kIntRegistryValues) / sizeof(kIntRegistryValues[0]);
  if (key >= 0 && key < limited) return false;

  RUNTIME_ERROR("Int Registry Getter: out-of-range key", key);
  return true;
}

int IntRegistryGetter_get(int key) {
  if (IsInvalid(key)) return kNonsenseValue;

  return kIntRegistryValues[key];
}
