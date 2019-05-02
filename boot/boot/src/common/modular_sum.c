// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "modular_sum.h"

#include <stdbool.h>

#include "runtime_error.h"

static bool IsValidSize(int size) {
  if (size > 0 && !(size % 4)) return true;

  RUNTINE_ERROR("Modular Sum: invalid size", size);
  return false;
}

static uint32_t Sum(const uint32_t *data, int size) {
  uint32_t sum = 0;
  int loop = size / sizeof(uint32_t);
  for (int i = 0; i < loop; ++i) sum += data[i];

  return sum;
}

uint32_t ModularSum_Verify(const uint32_t *data, int size) {
  if (!IsValidSize(size)) return 1;

  return Sum(data, size);
}

uint32_t ModularSum_Calculate(const uint32_t *data, int size) {
  if (!IsValidSize(size)) return 0;

  return ~Sum(data, size) + 1;
}
