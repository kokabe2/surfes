// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "registry_api.h"

#include "database/int_registry_getter.h"
#include "database/uintptr_registry_getter.h"

const RegistryApiStruct kRegistryApi = {
    .getIntRegistryValue = IntRegistryGetter_get,
    .getUintptrRegistryValue = UintptrRegistryGetter_get,
};

RegistryApi RegistryApi_getDefaultInstance(void) {
  return (RegistryApi)&kRegistryApi;
}
