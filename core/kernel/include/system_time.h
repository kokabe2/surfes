// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_INCLUDE_SYSTEM_TIME_H_
#define CORE_KERNEL_INCLUDE_SYSTEM_TIME_H_

#include <stdint.h>

void SystemTime_Create(uint64_t current_time);
void SystemTime_Destroy(void);
uint64_t SystemTime_get(void);

#endif  // CORE_KERNEL_INCLUDE_SYSTEM_TIME_H_
