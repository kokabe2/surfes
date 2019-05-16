// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "sif_file.h"

#include <stdbool.h>
#include <stddef.h>

#include "sif_file_validator.h"
#include "sif_header.h"
#include "utilities/instance_helper.h"
#include "utilities/runtime_error.h"

typedef struct SifFileStruct {
  SifHeader header;
  exeEntryPoint Execute;
  libEntryPoint primary_api;
} SifFileStruct;

static SifFile NewInstance(SifHeader header) {
  SifFile self = (SifFile)InstanceHelper_New(sizeof(SifFileStruct));
  if (self) {
    self->header = header;
    self->Execute = (exeEntryPoint)header->entry_point;
    self->primary_api = (libEntryPoint)header->entry_point;
  }

  return self;
}

static int CallOpenIfNeeded(SifHeader header) {
  PrimaryApi primary_api = (PrimaryApi)header->primary_api_address;
  if (primary_api && primary_api->Open) return primary_api->Open();

  return 0;
}

SifFile SifFile_Open(uintptr_t file_address) {
  if (SifFileValidator_Validate(file_address)) return NULL;
  if (CallOpenIfNeeded((SifHeader)file_address)) return NULL;

  return NewInstance((SifHeader)file_address);
}

static void CallCloseIfNeeded(SifFile self) {
  if (self->primary_api && self->primary_api->Close) self->primary_api->Close();
}

void SifFile_Close(SifFile* self) {
  if (!self || !*self) return;

  CallCloseIfNeeded(*self);
  InstanceHelper_Delete(self);
}

static bool IsValid(SifFile self) {
  if (self) return true;

  RUNTIME_ERROR("SIF File: null instance", 0);
  return false;
}

uint64_t SifFile_GetVersion(SifFile self) {
  if (!IsValid(self)) return 0;

  return self->header->file_version;
}

int SifFile_Execute(SifFile self, int runlevel) {
  if (IsValid(self) && self->Execute) return self->Execute(runlevel);

  return 0;
}

int SifFile_Write(SifFile self, int id, const void* data) {
  if (IsValid(self) && self->primary_api && self->primary_api->Write)
    return self->primary_api->Write(id, data);

  return 0;
}

int SifFile_Read(SifFile self, int id, void* data) {
  if (IsValid(self) && self->primary_api && self->primary_api->Read)
    return self->primary_api->Read(id, data);

  return 0;
}

int SifFile_Control(SifFile self, int id, void* data) {
  if (IsValid(self) && self->primary_api && self->primary_api->Control)
    return self->primary_api->Control(id, data);

  return 0;
}
