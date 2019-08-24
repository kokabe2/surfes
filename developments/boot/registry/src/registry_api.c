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

static SifFile its_file;
static RegistryApi NewInstance(uintptr_t file_address) {
  its_file = SifFile_Open(file_address);
  RegistryApi instance = (RegistryApi)SifFile_getEntryPoint(its_file);
  if (instance) return instance;

  return (RegistryApi)&kRegistryApi;
}

static RegistryApi its_instance = NULL;
void RegistryApi_Create(uintptr_t file_address) {
  its_instance = NewInstance(file_address);
}

RegistryApi RegistryApi_getInstance(void) {
  if (!its_instance) its_instance = NewInstance(kRegistryFileAddress);

  return its_instance;
}

void RegistryApi_Destroy(void) {
  SifFile_Close(&its_file);
  its_instance = NULL;
}
