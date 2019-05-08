// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "system_runner.h"
#include "system_run_service_interface.h"

#include <stdbool.h>

#include "core_file.h"
#include "updater_file.h"

enum {
  kLowestRunlevel = 0,
  kHighestRunlevel = 6,
};

static int default_runlevel = kLowestRunlevel;

void SystemRunner_Create(int runlevel) { default_runlevel = runlevel; }

void SystemRunner_Destroy(void) { default_runlevel = kLowestRunlevel; }

static bool IsValid(int runlevel) {
  return (runlevel >= kLowestRunlevel) && (runlevel <= kHighestRunlevel);
}

static bool IsNotHalt(int runlevel) { return runlevel != kLowestRunlevel; }

static int RunCore(int runlevel) {
  SifFile* file = CoreFile_getInstance();
  return SifFile_Execute(*file, runlevel);
}

static int RunUpdater(int runlevel) {
  SifFile* file = UpdaterFile_getInstance();
  return SifFile_Execute(*file, runlevel);
}

static int Reboot(int unused) {
  SystemRunServiceInterface_Reboot();
  return kLowestRunlevel;
}

typedef int (*runner)(int);
static const runner kRunners[kHighestRunlevel] = {
    RunUpdater, RunCore, RunCore, RunCore, RunCore, Reboot,
};
void SystemRunner_Run(void) {
  int runlevel = 3;
  while (IsValid(runlevel) && IsNotHalt(runlevel))
    runlevel = kRunners[runlevel - 1](runlevel);

  SystemRunServiceInterface_Halt();
}
