// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_INT_H_
#define CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_INT_H_

#include "utkernel.h"

void fake_int_init(void);
FP fake_int_getIsrFunction(UINT intno);
ATR fake_int_getAttribute(UINT intno);
INT fake_int_getLevel(UINT intno);
void fake_int_run(UINT intno);

#endif  // CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_INT_H_
