// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "updater_file.h"

#include <stddef.h>

enum {
  kUpdaterFileAddress = 0xFFC00000,
};

static SifFile instance = NULL;

static SifFile UpdaterFile_New(uintptr_t file_address) {
  return SifFile_Open(file_address);
}

void UpdaterFile_Create(uintptr_t file_address) {
  UpdaterFile_Destroy();

  instance = UpdaterFile_New(file_address);
}

void UpdaterFile_Destroy(void) { SifFile_Close(&instance); }

SifFile* UpdaterFile_getInstance(void) {
  if (!instance) instance = UpdaterFile_New(kUpdaterFileAddress);

  return &instance;
}
