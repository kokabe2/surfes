// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "system_executor_factory_impl.h"

#include <stdbool.h>
#include <stddef.h>

#include "halt_system.h"
#include "immutable_registry.h"
#include "reboot_system.h"
#include "runtime_error.h"
#include "user_system.h"

static bool IsInvalid(int runlevel) {
  if (runlevel >= kLowestRunlevel && runlevel <= kHighestRunlevel) return false;

  RUNTIME_ERROR("System Executor Factory Impl: out-of-range runlevel",
                runlevel);
  return true;
}

static ISystemExecutable Make(int runlevel) {
  if (IsInvalid(runlevel)) return NULL;

  if (runlevel == kLowestRunlevel) return HaltSystem_getInstance();
  if (runlevel == kHighestRunlevel) return RebootSystem_getInstance();

  return UserSystem_getExecutor(runlevel);
}

static ISystemExecutorFactoryStruct its_instance = {
    .Make = Make,
};
ISystemExecutorFactory SystemExecutorFactoryImpl_getInstance(void) {
  return &its_instance;
}
