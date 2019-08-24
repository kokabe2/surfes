// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_SCRIPT_SYSTEM_SCRIPT_H_
#define BOOT_BOOT_SRC_SCRIPT_SYSTEM_SCRIPT_H_

#include "system_executor_factory.h"

void SystemScript_Create(ISystemExecutorFactory factory);
void SystemScript_Destroy(void);
void SystemScript_Run(int runlevel);

#endif  // BOOT_BOOT_SRC_SCRIPT_SYSTEM_SCRIPT_H_
