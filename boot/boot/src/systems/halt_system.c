// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "halt_system.h"
#include "halt_system_service_interface.h"

static int Execute(void) {
  HaltSystemServiceInterface_Shutdown();  // never returns in production code
  return -1;
}

static ISystemExecutableStruct its_instance = {
    .Execute = Execute,
};
ISystemExecutable HaltSystem_getInstance(void) { return &its_instance; }
