// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "system_script.h"

#include <stddef.h>

#include "script/system_executable.h"

static ISystemExecutorFactory its_factory = NULL;

void SystemScript_Create(ISystemExecutorFactory factory) {
  its_factory = factory;
}

void SystemScript_Destroy(void) { its_factory = NULL; }

void SystemScript_Run(int runlevel) {
  if (!its_factory) return;

  ISystemExecutable se;
  while ((se = its_factory->Make(runlevel)) != NULL) runlevel = se->Execute();
}
