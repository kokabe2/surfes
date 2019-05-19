// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "user_system_registry.h"

#include <stddef.h>

#include "user_system_registry_file.h"
#include "user_system_registry_file_interface.h"

static uintptr_t its_address = kUserSystemRegistryFileAddress;
void UserSystemRegistry_Change(uintptr_t address) { its_address = address; }

static UserSystemRegistryFile its_file = NULL;
ISystemExecutable UserSystemRegistry_getExecutor(int runlevel) {
  if (!its_file) its_file = UserSystemRegistryFile_Open(its_address);

  return UserSystemRegistryFile_getExecutor(its_file, runlevel);
}
