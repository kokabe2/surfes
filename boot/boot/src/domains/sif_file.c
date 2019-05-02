// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "sif_file.h"

#include <stddef.h>
#include <stdlib.h>

#include "common/instance_helper.h"
#include "sif_file_validator.h"
#include "sif_header.h"

typedef struct {
  SifFileStruct base;
  void (*Close)(void);
} SifFilePrivateStruct, *SifFilePrivate;

static int default_Execute(void) { return 0; }
static int default_Write(int id, const void* data) { return 0; }
static int default_Read(int id, void* data) { return 0; }
static int default_Control(int id, void* data) { return 0; }
static void InstallDefalutInterface(SifHeader header, SifFilePrivate self) {
  self->base.size = header->file_size;
  self->base.version = header->file_version;
  self->base.Execute = default_Execute;
  self->base.Write = default_Write;
  self->base.Read = default_Read;
  self->base.Control = default_Control;
}

typedef int (*executor)(void);
static void UpdateEntryPointIfNeeded(SifHeader header, SifFilePrivate self) {
  if (header->entry_point) self->base.Execute = (executor)header->entry_point;
}

typedef struct {
  int (*Open)(void);
  void (*Close)(void);
  int (*Write)(int id, const void* data);
  int (*Read)(int id, void* data);
  int (*Control)(int id, void* data);
} * PrimaryApiTable;
static void UpdatePrimaryApiIfNeeded(SifHeader header, SifFilePrivate self) {
  PrimaryApiTable api_table = (PrimaryApiTable)header->primary_api_address;
  if (!api_table) return;
  if (api_table->Write) self->base.Write = api_table->Write;
  if (api_table->Read) self->base.Read = api_table->Read;
  if (api_table->Control) self->base.Control = api_table->Control;
  if (api_table->Close) self->Close = api_table->Close;
}

static void InstallInterface(SifHeader header, SifFilePrivate self) {
  InstallDefalutInterface(header, self);
  UpdateEntryPointIfNeeded(header, self);
  UpdatePrimaryApiIfNeeded(header, self);
}

static int CallOpenIfNeeded(SifHeader header) {
  PrimaryApiTable api_table = (PrimaryApiTable)header->primary_api_address;
  if (api_table && api_table->Open) return api_table->Open();
  return 0;
}

SifFile SifFile_Open(uintptr_t file_address) {
  if (SifFileValidator_Validate(file_address)) return NULL;

  SifFilePrivate self =
      (SifFilePrivate)InstanceHelper_New(sizeof(SifFilePrivateStruct));
  if (!self) return NULL;

  SifHeader header = (SifHeader)file_address;
  InstallInterface(header, self);
  if (CallOpenIfNeeded(header)) InstanceHelper_Delete(&self);

  return (SifFile)self;
}

static void CallCloseIfNeeded(SifFilePrivate self) {
  if (self && self->Close) self->Close();
}

void SifFile_Close(SifFile* self) {
  if (!self) return;

  CallCloseIfNeeded((SifFilePrivate)*self);
  InstanceHelper_Delete(self);
}
