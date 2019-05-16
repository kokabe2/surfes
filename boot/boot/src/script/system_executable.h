// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_SCRIPT_SYSTEM_EXECUTABLE_H_
#define BOOT_BOOT_SRC_SCRIPT_SYSTEM_EXECUTABLE_H_

typedef struct {
  int (*Execute)(void);
} ISystemExecutableStruct, *ISystemExecutable;

#endif  // BOOT_BOOT_SRC_SCRIPT_SYSTEM_EXECUTABLE_H_
