// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_UTILITY_INC_MODULAR_SUM_H_
#define DEVELOPMENTS_UTILITY_INC_MODULAR_SUM_H_

#include <stdint.h>

uint32_t ModularSum_Verify(const uint32_t *data, int size);
uint32_t ModularSum_Calculate(const uint32_t *data, int size);

#endif  // DEVELOPMENTS_UTILITY_INC_MODULAR_SUM_H_
