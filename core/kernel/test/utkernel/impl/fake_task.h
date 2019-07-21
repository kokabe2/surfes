// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_TASK_H_
#define CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_TASK_H_

#include <stdbool.h>

#include "utkernel.h"

#define TTS_NONE 0x00000000
#define TTS_RUN 0x00000001
#define TTS_RDY 0x00000002
#define TTS_WAI 0x00000004
#define TTS_SUS 0x00000008
#define TTS_WAS 0x0000000c
#define TTS_DMT 0x00000010

void fake_task_init(void);
ATR fake_task_getAttribute(ID tskid);
PRI fake_task_getPriority(ID tskid);
SZ fake_task_getStackSize(ID tskid);
UINT fake_task_getState(ID tskid);
void fake_task_setState(ID tskid, UINT state);
TMO fake_task_getTimeout(ID tskid);
INT fake_task_getSuspendCount(ID tskid);
INT fake_task_getWakeupCount(ID tskid);
bool fake_task_dispatch(ID tskid);

#endif  // CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_TASK_H_
