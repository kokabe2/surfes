// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "modular_sum.h"

#include <stdbool.h>

inline static bool IsMultiplesOfFour(int size) { return (size % 4) == 0; }

inline static bool IsValid(int size) {
  return size > 0 && IsMultiplesOfFour(size);
}

static uint32_t Sum(const uint32_t *data, int size) {
  uint32_t sum = 0;
  int loop = size / sizeof(uint32_t);
  for (int i = 0; i < loop; ++i) sum += data[i];

  return sum;
}

uint32_t ModularSum_Verify(const uint32_t *data, int size) {
  return IsValid(size) ? Sum(data, size) : ~0;
}

uint32_t ModularSum_Calculate(const uint32_t *data, int size) {
  return IsValid(size) ? ~Sum(data, size) + 1 : 0;
}
