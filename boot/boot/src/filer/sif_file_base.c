// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "sif_file_base.h"

#include <stddef.h>

#include "sif_file_validator.h"
#include "utilities/instance_helper.h"

static SifFileBase NewInstance(SifHeader header, int size) {
  SifFileBase self = (SifFileBase)InstanceHelper_New(size);
  if (self) {
    self->version = header->file_version;
    self->Close = (closeFunction)header->close_function_address;
  }

  return self;
}

static int CallOpenIfNeeded(SifHeader header) {
  openFunction Open = (openFunction)header->open_function_address;
  if (Open) return Open();

  return 0;
}

SifFileBase SifFileBase_Open(uintptr_t file_address, int instance_size) {
  if (SifFileValidator_Validate(file_address)) return NULL;
  if (CallOpenIfNeeded((SifHeader)file_address)) return NULL;

  return NewInstance((SifHeader)file_address, instance_size);
}

static void CallCloseIfNeeded(SifFileBase self) {
  if (self && self->Close) self->Close();
}

void SifFileBase_Close(SifFileBase* self) {
  if (!self) return;

  CallCloseIfNeeded(*self);
  InstanceHelper_Delete(self);
}

uint64_t SifFileBase_getVersion(SifFileBase self) {
  if (self) return self->version;

  return 0;
}
