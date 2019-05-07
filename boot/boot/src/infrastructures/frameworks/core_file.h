// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_INFRASTRUCTURES_FRAMEWORKS_CORE_FILE_H_
#define BOOT_BOOT_SRC_INFRASTRUCTURES_FRAMEWORKS_CORE_FILE_H_

#include "sif_file.h"

void CoreFile_Create(uintptr_t file_address);
void CoreFile_Destroy(void);
SifFile* CoreFile_getInstance(void);

#endif  // BOOT_BOOT_SRC_INFRASTRUCTURES_FRAMEWORKS_CORE_FILE_H_
