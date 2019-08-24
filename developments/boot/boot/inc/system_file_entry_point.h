// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_INCLUDE_SYSTEM_FILE_ENTRY_POINT_H_
#define BOOT_BOOT_INCLUDE_SYSTEM_FILE_ENTRY_POINT_H_

typedef struct SystemFileEntryPointStruct {
  int (*Execute)(int runlevel);  // Returns next runlevel
} SystemFileEntryPointStruct, *SystemFileEntryPoint;

#endif  // BOOT_BOOT_INCLUDE_SYSTEM_FILE_ENTRY_POINT_H_
