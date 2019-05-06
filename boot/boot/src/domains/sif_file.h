// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_DOMAINS_SIF_FILE_H_
#define BOOT_BOOT_SRC_DOMAINS_SIF_FILE_H_

#include <stdint.h>

typedef struct SifFileStruct *SifFile;

SifFile SifFile_Open(uintptr_t file_address);
uint64_t SifFile_GetVersion(SifFile self);
int SifFile_Execute(SifFile self, int runlevel);
int SifFile_Write(SifFile self, int id, const void *data);
int SifFile_Read(SifFile self, int id, void *data);
int SifFile_Control(SifFile self, int id, void *data);
void SifFile_Close(SifFile *self);

#endif  // BOOT_BOOT_SRC_DOMAINS_SIF_FILE_H_
