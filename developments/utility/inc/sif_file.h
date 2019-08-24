// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_UTILITY_INC_SIF_FILE_H_
#define DEVELOPMENTS_UTILITY_INC_SIF_FILE_H_

#include <stdint.h>

typedef struct SifFileStruct* SifFile;

SifFile SifFile_Open(uintptr_t file_address);
void SifFile_Close(SifFile* self);
uint64_t SifFile_getVersion(SifFile self);
uintptr_t SifFile_getEntryPoint(SifFile self);

#endif  // DEVELOPMENTS_UTILITY_INC_SIF_FILE_H_
