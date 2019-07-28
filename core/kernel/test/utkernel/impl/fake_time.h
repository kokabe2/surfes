// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_TIME_H_
#define CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_TIME_H_

#include <stdint.h>

void fake_time_init(void);
void fake_time_increment(uint64_t time);

#endif  // CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_TIME_H_
