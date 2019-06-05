// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "runlevel_provider.h"

#include "registry_api.h"

int RunlevelProvider_getDefaultRunlevel(void) {
  RegistryApi ra = RegistryApi_getInstance();
  return ra->getIntRegistryValue(kIrkDefaultRunlevel);
}
