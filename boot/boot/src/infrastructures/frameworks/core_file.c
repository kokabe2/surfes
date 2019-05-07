// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "core_file.h"

#include <stddef.h>

enum {
  kCoreFileAddress = 0xFFC00000,
};

static SifFile instance = NULL;

static SifFile CoreFile_New(uintptr_t file_address) {
  return SifFile_Open(file_address);
}

void CoreFile_Create(uintptr_t file_address) {
  CoreFile_Destroy();

  instance = CoreFile_New(file_address);
}

void CoreFile_Destroy(void) { SifFile_Close(&instance); }

SifFile* CoreFile_getInstance(void) {
  if (!instance) instance = CoreFile_New(kCoreFileAddress);

  return &instance;
}
