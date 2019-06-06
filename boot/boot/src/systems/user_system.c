// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "user_system.h"

#include <stddef.h>

#include "sif_file.h"
#include "system_file_entry_point.h"

static IUserSystemFileProvider its_provider = NULL;
void UserSystem_Create(IUserSystemFileProvider provider) {
  its_provider = provider;
}

void UserSystem_Destroy(void) { its_provider = NULL; }

static int its_runlevel;
static int Execute(void) {
  int next_runlevel = -1;

  uintptr_t file_address = its_provider->getFileAddress(its_runlevel);
  SifFile sf = SifFile_Open(file_address);
  SystemFileEntryPoint sfep = (SystemFileEntryPoint)SifFile_getEntryPoint(sf);
  if (sfep) next_runlevel = sfep->Execute(its_runlevel);
  SifFile_Close(&sf);

  return next_runlevel;
}

static ISystemExecutableStruct its_instance = {
    .Execute = Execute,
};
ISystemExecutable UserSystem_getExecutor(int runlevel) {
  if (!its_provider) return NULL;

  its_runlevel = runlevel;

  return &its_instance;
}
