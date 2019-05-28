// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_SCRIPT_SYSTEM_EXECUTOR_FACTORY_H_
#define BOOT_BOOT_SRC_SCRIPT_SYSTEM_EXECUTOR_FACTORY_H_

#include "script/system_executable.h"

typedef struct {
  ISystemExecutable (*Make)(int runlevel);
} ISystemExecutorFactoryStruct, *ISystemExecutorFactory;

#endif  // BOOT_BOOT_SRC_SCRIPT_SYSTEM_EXECUTOR_FACTORY_H_
