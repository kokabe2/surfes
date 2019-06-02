// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_SYSTEMS_USER_SYSTEM_H_
#define BOOT_BOOT_SRC_SYSTEMS_USER_SYSTEM_H_

#include "script/system_executable.h"
#include "user_system_file_provider.h"

void UserSystem_Create(IUserSystemFileProvider provider);
void UserSystem_Destroy(void);
ISystemExecutable UserSystem_getExecutor(int runlevel);

#endif  // BOOT_BOOT_SRC_SYSTEMS_USER_SYSTEM_H_
