// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "reboot_system.h"
#include "reboot_system_service_interface.h"

static int Execute(void) {
  RebootSystemServiceInterface_Reboot();  // never returns in production code
  return -1;
}

static ISystemExecutableStruct its_instance = {
    .Execute = Execute,
};
ISystemExecutable RebootSystem_getInstance(void) { return &its_instance; }
