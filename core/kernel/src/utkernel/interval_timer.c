// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "interval_timer.h"

#include <stdbool.h>
#include <stddef.h>

#include "impl/utkernel.h"
#include "instance_helper.h"
#include "timer_private.h"

static bool Validate(ScheduledFunction function, int time_in_milliseconds) {
  return function && (time_in_milliseconds >= 0);
}

static void TimerEntry(void* exinf) {
  Timer self = (Timer)exinf;
  self->function(self->parameter);
}

static bool CreateTimer(Timer self) {
  T_CCYC packet = {
      .exinf = self,
      .cycatr = (TA_HLNG | TA_STA | TA_PHS),
      .cychdr = TimerEntry,
      .cyctim = self->base_time,
      .cycphs = self->base_time,
  };
  return (self->id = tk_cre_cyc(&packet)) >= 0;
}

static void Destroy(Timer* self) {
  tk_del_cyc((*self)->id);
  InstanceHelper_Delete(self);
}

static void Resume(Timer self);
static void Pause(Timer self) {
  // TODO(okabe): Use a critical section
  self->Pause = NULL;
  tk_stp_cyc(self->id);
  self->Resume = Resume;
}

static Timer NewInstance(ScheduledFunction function, int time_in_milliseconds,
                         void* parameter) {
  Timer self = (Timer)InstanceHelper_New(sizeof(TimerStruct));
  if (!self) return NULL;

  self->base_time = time_in_milliseconds;
  self->function = function;
  self->parameter = parameter;
  self->Destroy = Destroy;
  self->Pause = Pause;

  if (!CreateTimer(self)) InstanceHelper_Delete(self);

  return self;
}

Timer IntervalTimer_Create(ScheduledFunction function, int time_in_milliseconds,
                           void* parameter) {
  return Validate(function, time_in_milliseconds)
             ? NewInstance(function, time_in_milliseconds, parameter)
             : NULL;
}

static void Resume(Timer self) {
  // TODO(okabe): Use a critical section
  self->Resume = NULL;
  tk_sta_cyc(self->id);
  self->Pause = Pause;
}
