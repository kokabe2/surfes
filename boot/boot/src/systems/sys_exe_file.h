// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_SYSTEMS_SYS_EXE_FILE_H_
#define BOOT_BOOT_SRC_SYSTEMS_SYS_EXE_FILE_H_

#include <stdint.h>

typedef struct SysExeFileStruct *SysExeFile;

SysExeFile SysExeFile_Open(uintptr_t file_address);
void SysExeFile_Close(SysExeFile *self);
int SysExeFile_Execute(SysExeFile self, int parameter);

#endif  // BOOT_BOOT_SRC_SYSTEMS_SYS_EXE_FILE_H_
