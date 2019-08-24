// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "sif_file.h"

#include <stddef.h>

#include "instance_helper.h"
#include "sif_file_validator.h"
#include "sif_header.h"

typedef struct SifFileStruct {
  uint64_t version;
  uintptr_t entry_point;
  closeFunction Close;
} SifFileStruct;

static int CallOpenIfNeeded(SifHeader header) {
  openFunction Open = (openFunction)header->open_function_address;
  return Open ? Open() : 0;
}

static SifFile NewInstance(SifHeader header) {
  SifFile self = (SifFile)InstanceHelper_New(sizeof(SifFileStruct));
  if (self) {
    self->version = header->file_version;
    self->entry_point = header->entry_point;
    self->Close = (closeFunction)header->close_function_address;
  }
  return self;
}

SifFile SifFile_Open(uintptr_t file_address) {
  if (SifFileValidator_Validate(file_address)) return NULL;
  if (CallOpenIfNeeded((SifHeader)file_address)) return NULL;

  return NewInstance((SifHeader)file_address);
}

static void CallCloseIfNeeded(SifFile self) {
  if (self && self->Close) self->Close();
}

void SifFile_Close(SifFile* self) {
  if (!self) return;

  CallCloseIfNeeded(*self);
  InstanceHelper_Delete(self);
}

uint64_t SifFile_getVersion(SifFile self) {
  if (self) return self->version;

  return 0;
}

uintptr_t SifFile_getEntryPoint(SifFile self) {
  if (self) return self->entry_point;

  return 0;
}
