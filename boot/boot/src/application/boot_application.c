// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "boot_application.h"

#include "registries/user_system_registry.h"
#include "script/system_script.h"
#include "systems/system_executor_factory_impl.h"

void BootApplication_Run(void) {
  ISystemExecutorFactory factory = SystemExecutorFactoryImpl_getInstance();
  SystemScript_Create(factory);

  int runlevel = UserSystemRegistry_getDefaultRunlevel();
  SystemScript_Run(runlevel);
}
