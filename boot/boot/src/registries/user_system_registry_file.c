// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "user_system_registry_file.h"
#include "user_system_registry_file_interface.h"

#include <stddef.h>

#include "sif_file_base.h"

enum ValidationErrors {
  kNoError = 0,
  kFileTypeError,
  kEntryPointError,
};

typedef struct UserSystemRegistryFileStruct {
  SifFileBaseStruct base;
  UserSystemRegistryFileEntryPoint entry_point;
} UserSystemRegistryFileStruct;

static int Validate(SifHeader header) {
  if (header->type != kStData) return kFileTypeError;

  UserSystemRegistryFileEntryPoint entry_point =
      (UserSystemRegistryFileEntryPoint)header->entry_point;
  if (!entry_point || !entry_point->getExecutor) return kEntryPointError;

  return kNoError;
}

static void SetField(SifHeader header, UserSystemRegistryFile self) {
  self->entry_point = (UserSystemRegistryFileEntryPoint)header->entry_point;
}

UserSystemRegistryFile UserSystemRegistryFile_Open(uintptr_t file_address) {
  if (Validate((SifHeader)file_address)) return NULL;

  UserSystemRegistryFile self = (UserSystemRegistryFile)SifFileBase_Open(
      file_address, sizeof(UserSystemRegistryFileStruct));
  if (self) SetField((SifHeader)file_address, self);

  return self;
}

void UserSystemRegistryFile_Close(UserSystemRegistryFile *self) {
  SifFileBase_Close((SifFileBase *)self);
}

ISystemExecutable UserSystemRegistryFile_getExecutor(
    UserSystemRegistryFile self, int runlevel) {
  if (self) return self->entry_point->getExecutor(runlevel);

  return NULL;
}
