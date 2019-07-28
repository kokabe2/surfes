// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_INCLUDE_INTERVAL_TIMER_H_
#define CORE_KERNEL_INCLUDE_INTERVAL_TIMER_H_

#include "timer.h"

Timer IntervalTimer_Create(ScheduledFunction function, int time_in_milliseconds,
                           void* parameter);

#endif  // CORE_KERNEL_INCLUDE_INTERVAL_TIMER_H_
