// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "boot_application.h"

#include "script/system_script.h"
#include "systems/system_executor_factory_impl.h"

enum {
  kDefaultRunlevel = 3,
};

void BootApplication_Run(void) {
  ISystemExecutorFactory factory = SystemExecutorFactoryImpl_getInstance();
  SystemScript_Create(factory);
  SystemScript_Run(kDefaultRunlevel);
}
