// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_INCLUDE_TIMER_H_
#define CORE_KERNEL_INCLUDE_TIMER_H_

typedef void (*ScheduledFunction)(void* parameter);
typedef struct TimerStruct* Timer;

// Abstruct Timer Timer_Create();
void Timer_Destroy(Timer* self);
void Timer_Pause(Timer self);
void Timer_Resume(Timer self);

#endif  // CORE_KERNEL_INCLUDE_TIMER_H_
