// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "timer.h"

#include "timer_private.h"

void Timer_Destroy(Timer* self) {
  if (self && *self) (*self)->Destroy(self);
}

void Timer_Pause(Timer self) {
  if (self && self->Pause) self->Pause(self);
}

void Timer_Resume(Timer self) {
  if (self && self->Resume) self->Resume(self);
}
