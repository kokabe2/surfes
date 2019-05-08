// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "infrastructures/frameworks/system_run_service_interface.h"

#include "microcontroller.h"

void SystemRunServiceInterface_Halt(void) {
  for (;;) continue;
}

void SystemRunServiceInterface_Reboot(void) { Microcontroller_Reset(); }
