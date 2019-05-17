// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "application/boot_application.h"
#include "platforms/platform.h"

void BootStrapper(void) {
  Platform_Initialize();
  BootApplication_Run();
  Platform_Abort();
}
