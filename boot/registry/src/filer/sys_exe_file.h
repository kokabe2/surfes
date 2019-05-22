// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_REGISTRY_SRC_FILER_SYS_EXE_FILE_H_
#define BOOT_REGISTRY_SRC_FILER_SYS_EXE_FILE_H_

#include <stdint.h>

typedef struct SysExeFileStruct *SysExeFile;

SysExeFile SysExeFile_Open(uintptr_t file_address);
void SysExeFile_Close(SysExeFile *self);
int SysExeFile_Execute(SysExeFile self, int parameter);

#endif  // BOOT_REGISTRY_SRC_FILER_SYS_EXE_FILE_H_
