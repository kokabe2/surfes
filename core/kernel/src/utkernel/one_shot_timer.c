﻿// Copyright(c) 2019 Ken Okabe
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

static void TimerEntry(void* exinf) {
  Timer self = (Timer)exinf;
  self->function(self->parameter);
  self->Suspend = NULL;
  self->Resume = NULL;
}

static bool CreateTimer(Timer self) {
  T_CALM packet = {
      .exinf = self,
      .almatr = TA_HLNG,
      .almhdr = TimerEntry,
  };
  return (self->id = tk_cre_alm(&packet)) >= 0;
}

static void Destroy(Timer* self) {
  tk_del_alm((*self)->id);
  InstanceHelper_Delete(self);
}

static void UpdateBaseTimeToLeftTime(Timer self) {
  T_RALM packet;
  tk_ref_alm(self->id, &packet);
  self->base_time = packet.lfttim;
}

static void Resume(Timer self);
static void Suspend(Timer self) {
  self->Suspend = NULL;
  UpdateBaseTimeToLeftTime(self);
  tk_stp_alm(self->id);
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
  self->Suspend = Suspend;

  if (!CreateTimer(self)) InstanceHelper_Delete(self);

  return self;
}

static void ScheduleTimer(Timer self) {
  tk_sta_alm(self->id, (RELTIM)self->base_time);
}

Timer OneShotTimer_Create(ScheduledFunction function, int time_in_milliseconds,
                          void* parameter) {
  if (!Validate(function, time_in_milliseconds)) return NULL;

  Timer self = NewInstance(function, time_in_milliseconds, parameter);
  if (self) ScheduleTimer(self);
  return self;
}

static void Resume(Timer self) {
  self->Resume = NULL;
  ScheduleTimer(self);
  self->Suspend = Suspend;
}
