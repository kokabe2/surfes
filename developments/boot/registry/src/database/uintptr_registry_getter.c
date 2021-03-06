﻿// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "uintptr_registry_getter.h"

#include <stdbool.h>

#include "mutable_registry.h"
#include "runtime_error.h"

enum {
  kNonsenseValue = 0,
};

static const uintptr_t kUintptrRegistryValues[] = {
    kCoreFileAddress,           kUpdaterFileAddress,
    kRecoverySystemFileAddress, kUpdateSystemFileAddress,
    kNormalSystemFileAddress,   kAdvancedSystemFileAddress,
};

static bool IsInvalid(int key) {
  int limited =
      sizeof(kUintptrRegistryValues) / sizeof(kUintptrRegistryValues[0]);
  if (key >= 0 && key < limited) return false;

  RUNTIME_ERROR("Uintptr Registry Getter: out-of-range key", key);
  return true;
}

uintptr_t UintptrRegistryGetter_get(int key) {
  if (IsInvalid(key)) return kNonsenseValue;

  return kUintptrRegistryValues[key];
}
