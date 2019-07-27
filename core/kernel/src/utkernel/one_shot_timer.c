// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "one_shot_timer.h"

#include <stdbool.h>
#include <stddef.h>

#include "impl/utkernel.h"
#include "instance_helper.h"
#include "timer_private.h"

static bool Validate(ScheduledFunction function, int time_in_milliseconds) {
  return function && (time_in_milliseconds >= 0);
}

static bool CreateTimer(Timer self, ScheduledFunction function,
                        void* parameter) {
  T_CALM packet = {
      .exinf = parameter,
      .almatr = TA_HLNG,
      .almhdr = function,
  };
  return (self->id = tk_cre_alm(&packet)) >= 0;
}

static void Destroy(Timer* self) {
  tk_del_alm((*self)->id);
  InstanceHelper_Delete(self);
}

static void Suspend(Timer self) { tk_stp_alm(self->id); }

static void ScheduleTimer(Timer self) {
  tk_sta_alm(self->id, (RELTIM)self->time_in_milliseconds);
}

static Timer NewInstance(ScheduledFunction function, int time_in_milliseconds,
                         void* parameter) {
  Timer self = (Timer)InstanceHelper_New(sizeof(TimerStruct));
  if (!self) return NULL;

  if (CreateTimer(self, function, parameter)) {
    self->time_in_milliseconds = time_in_milliseconds;
    self->Destroy = Destroy;
    self->Suspend = Suspend;
    self->Resume = ScheduleTimer;
  } else {
    InstanceHelper_Delete(self);
  }

  return self;
}

Timer OneShotTimer_Create(ScheduledFunction function, int time_in_milliseconds,
                          void* parameter) {
  if (!Validate(function, time_in_milliseconds)) return NULL;

  Timer self = NewInstance(function, time_in_milliseconds, parameter);
  if (self) ScheduleTimer(self);
  return self;
}
