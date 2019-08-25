// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_UTILITY_INC_MICRO_TIME_H_
#define DEVELOPMENTS_UTILITY_INC_MICRO_TIME_H_

#include <stdint.h>

void MicroTime_Create(void);
uint64_t MicroTime_Get(void);
void MicroTime_Increase(uint64_t time);

#endif  // DEVELOPMENTS_UTILITY_INC_MICRO_TIME_H_
