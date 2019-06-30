// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef UTILITY_INCLUDE_MICRO_TIME_H_
#define UTILITY_INCLUDE_MICRO_TIME_H_

#include <stdint.h>

void MicroTime_Create(void);
uint64_t MicroTime_Get(void);
void MicroTime_Increase(uint64_t time);

#endif  // UTILITY_INCLUDE_MICRO_TIME_H_
