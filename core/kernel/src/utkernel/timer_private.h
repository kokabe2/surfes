// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_SRC_UTKERNEL_TIMER_PRIVATE_H_
#define CORE_KERNEL_SRC_UTKERNEL_TIMER_PRIVATE_H_

#include "timer.h"

typedef void (*SuspendFunction)(Timer);
typedef void (*ResumeFunction)(Timer);
typedef void (*DestroyFunction)(Timer*);

typedef struct TimerStruct {
  int id;
  int time_in_milliseconds;
  DestroyFunction Destroy;
  SuspendFunction Suspend;
  ResumeFunction Resume;
} TimerStruct;

#endif  // CORE_KERNEL_SRC_UTKERNEL_TIMER_PRIVATE_H_
