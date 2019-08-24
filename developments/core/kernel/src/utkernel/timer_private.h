// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_SRC_UTKERNEL_TIMER_PRIVATE_H_
#define CORE_KERNEL_SRC_UTKERNEL_TIMER_PRIVATE_H_

#include "timer.h"

typedef void (*PauseFunction)(Timer);
typedef void (*ResumeFunction)(Timer);
typedef void (*DestroyFunction)(Timer*);

typedef struct TimerStruct {
  int id;
  int base_time;
  ScheduledFunction function;
  void* parameter;
  DestroyFunction Destroy;
  PauseFunction Pause;
  ResumeFunction Resume;
} TimerStruct;

#endif  // CORE_KERNEL_SRC_UTKERNEL_TIMER_PRIVATE_H_
