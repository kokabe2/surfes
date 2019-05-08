// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_INFRASTRUCTURES_FRAMEWORKS_UPDATER_FILE_H_
#define BOOT_BOOT_SRC_INFRASTRUCTURES_FRAMEWORKS_UPDATER_FILE_H_

#include "sif_file.h"

void UpdaterFile_Create(uintptr_t file_address);
void UpdaterFile_Destroy(void);
SifFile* UpdaterFile_getInstance(void);

#endif  // BOOT_BOOT_SRC_INFRASTRUCTURES_FRAMEWORKS_UPDATER_FILE_H_
