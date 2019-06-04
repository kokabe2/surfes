// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "boot_application.h"

#include "registries/user_system_file_provider_impl.h"
#include "registries/user_system_registry.h"
#include "script/system_script.h"
#include "systems/system_executor_factory_impl.h"
#include "systems/user_system.h"

void BootApplication_Run(void) {
  IUserSystemFileProvider provider = UserSystemFileProviderImpl_getInstance();
  UserSystem_Create(provider);

  ISystemExecutorFactory factory = SystemExecutorFactoryImpl_getInstance();
  SystemScript_Create(factory);

  int runlevel = UserSystemRegistry_getDefaultRunlevel();
  SystemScript_Run(runlevel);
}
