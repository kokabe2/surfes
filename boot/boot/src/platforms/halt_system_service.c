// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "systems/halt_system_service_interface.h"

void HaltSystemServiceInterface_Halt(void) {
  for (;;) continue;
}
