// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_SYSTEMS_USER_SYSTEM_FILE_PROVIDER_H_
#define BOOT_BOOT_SRC_SYSTEMS_USER_SYSTEM_FILE_PROVIDER_H_

#include <stdint.h>

typedef struct IUserSystemFileProviderStruct {
  uintptr_t (*getFileAddress)(int runlevel);
} IUserSystemFileProviderStruct, *IUserSystemFileProvider;

#endif  // BOOT_BOOT_SRC_SYSTEMS_USER_SYSTEM_FILE_PROVIDER_H_
