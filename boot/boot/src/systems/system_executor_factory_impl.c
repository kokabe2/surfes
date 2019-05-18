// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "system_executor_factory_impl.h"

#include <stdbool.h>
#include <stddef.h>

#include "halt_system.h"
#include "reboot_system.h"
#include "update_system.h"
#include "user_system.h"
#include "runtime_error.h"

enum {
  kLowestRunlevel = 0,
  kHighestRunlevel = 6,
  kNumOfRunlevels = 7,
};

static bool IsInvalid(int runlevel) {
  if (runlevel >= kLowestRunlevel && runlevel <= kHighestRunlevel) return false;

  RUNTIME_ERROR("System Executor Factory Impl: out-of-range runlevel",
                runlevel);
  return true;
}

typedef ISystemExecutable (*systemExecutorGetter)(void);
const systemExecutorGetter kSystemExecutorGetters[kNumOfRunlevels] = {
    HaltSystem_getInstance,
    UpdateSystem_getInstance,
    NULL,
    UserSystem_getInstance,
    NULL,
    NULL,
    RebootSystem_getInstance,
};
static ISystemExecutable Make(int runlevel) {
  if (IsInvalid(runlevel)) return NULL;

  return kSystemExecutorGetters[runlevel]();
}

static ISystemExecutorFactoryStruct its_instance = {
    .Make = Make,
};
ISystemExecutorFactory SystemExecutorFactoryImpl_getInstance(void) {
  return &its_instance;
}
