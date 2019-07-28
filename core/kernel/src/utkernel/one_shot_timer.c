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

static bool UpdateBaseTimeToLeftTime(Timer self) {
  T_RALM packet;
  tk_ref_alm(self->id, &packet);
  if (packet.almstat == TALM_STP) return false;

  self->base_time = packet.lfttim;
  return true;
}

static void ScheduleTimer(Timer self) {
  tk_sta_alm(self->id, (RELTIM)self->base_time);
}

static void Resume(Timer self) {
  self->Resume = NULL;
  ScheduleTimer(self);
}

static void Suspend(Timer self) {
  if (!UpdateBaseTimeToLeftTime(self)) return;
  tk_stp_alm(self->id);
  self->Resume = Resume;
}

static Timer NewInstance(ScheduledFunction function, int time_in_milliseconds,
                         void* parameter) {
  Timer self = (Timer)InstanceHelper_New(sizeof(TimerStruct));
  if (!self) return NULL;

  if (CreateTimer(self, function, parameter)) {
    self->base_time = time_in_milliseconds;
    self->Destroy = Destroy;
    self->Suspend = Suspend;
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
