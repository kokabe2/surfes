// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "instance_helper.h"

#include <stdbool.h>
#include <stdlib.h>

void* InstanceHelper_New(int size) { return size > 0 ? calloc(1, size) : NULL; }

void InstanceHelper_Del(void** instance) {
  if (!instance || !*instance) return;

  free(*instance);
  *instance = NULL;
}
