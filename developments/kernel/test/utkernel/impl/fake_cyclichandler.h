// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_CYCLICHANDLER_H_
#define CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_CYCLICHANDLER_H_

#include <stdbool.h>

#include "utkernel.h"

void fake_cyclichandler_init(void);
ATR fake_cyclichandler_getAttribute(ID cycid);
RELTIM fake_cyclichandler_getLeftTime(ID cycid);
RELTIM fake_cyclichandler_getCyclicTime(ID cycid);
bool fake_cyclichandler_isActive(ID cycid);
bool fake_cyclichandler_isCreated(ID cycid);
void fake_cyclichandler_countdown(ID cycid, RELTIM time);

#endif  // CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_CYCLICHANDLER_H_
