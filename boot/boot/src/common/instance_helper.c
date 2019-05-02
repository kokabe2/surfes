// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "instance_helper.h"

#include <stdbool.h>
#include <stdlib.h>

#include "common/runtime_error.h"

static bool IsValidSize(int size) {
  if (size > 0) return true;

  RUNTINE_ERROR("Instance Helper: size is zero or less", size);
  return false;
}

void* InstanceHelper_New(int size) {
  if (!IsValidSize(size)) return NULL;

  return calloc(1, size);
}

void InstanceHelper_Del(void** instance) {
  if (!instance || !*instance) return;

  free(*instance);
  *instance = NULL;
}
