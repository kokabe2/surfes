// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "registry_api.h"
#include "registry_api_private.h"

#include <stddef.h>

#include "database/int_registry_getter.h"
#include "database/uintptr_registry_getter.h"
#include "immutable_registry.h"
#include "sif_file.h"

const RegistryApiStruct kRegistryApi = {
    .getIntRegistryValue = IntRegistryGetter_get,
    .getUintptrRegistryValue = UintptrRegistryGetter_get,
};

static uintptr_t registry_file_address = kRegistryFileAddress;
void RegistryApi_Change(uintptr_t file_address) {
  registry_file_address = file_address;
}

static SifFile its_file;
static RegistryApi NewInstance(void) {
  its_file = SifFile_Open(registry_file_address);
  RegistryApi instance = (RegistryApi)SifFile_getEntryPoint(its_file);
  if (instance) return instance;

  return (RegistryApi)&kRegistryApi;
}

static RegistryApi its_instance = NULL;
RegistryApi RegistryApi_getInstance(void) {
  if (!its_instance) its_instance = NewInstance();

  return its_instance;
}

void RegistryApi_Destroy(void) {
  SifFile_Close(&its_file);
  registry_file_address = kRegistryFileAddress;
  its_instance = NULL;
}
