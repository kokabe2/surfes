// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_ALARMHANDLER_H_
#define CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_ALARMHANDLER_H_

#include <stdbool.h>

#include "utkernel.h"

void fake_alarmhandler_init(void);
ATR fake_alarmhandler_getAttribute(ID almid);
RELTIM fake_alarmhandler_getLeftTime(ID almid);
bool fake_alarmhandler_isActive(ID almid);
bool fake_alarmhandler_isCreated(ID almid);
void fake_alarmhandler_countdown(ID almid, RELTIM time);

#endif  // CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_ALARMHANDLER_H_
