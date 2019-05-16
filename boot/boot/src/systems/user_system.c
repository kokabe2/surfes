// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "user_system.h"

#include <stddef.h>

#include "sys_exe_file.h"

enum {
  kCoreFileAddress = 0xFFE00000,
};

static uintptr_t its_file_address = kCoreFileAddress;
void UserSystem_Change(uintptr_t file_address) {
  its_file_address = file_address;
}

static SysExeFile its_file = NULL;
static int Execute(void) { return SysExeFile_Execute(its_file, 0); }

static ISystemExecutableStruct its_instance = {
    .Execute = Execute,
};
ISystemExecutable UserSystem_getInstance(void) {
  if (!its_file) its_file = SysExeFile_Open(its_file_address);

  return &its_instance;
}
