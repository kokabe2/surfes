// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_DOMAINS_SIF_FILE_H_
#define BOOT_BOOT_SRC_DOMAINS_SIF_FILE_H_

#include <stdint.h>

typedef struct {
  int size;
  uint64_t version;
  int (*Execute)(void);
  int (*Write)(int id, const void *data);
  int (*Read)(int id, void *data);
  int (*Control)(int id, void *data);
} SifFileStruct, *SifFile;

SifFile SifFile_Open(uintptr_t file_address);
void SifFile_Close(SifFile *self);

#endif  // BOOT_BOOT_SRC_DOMAINS_SIF_FILE_H_
