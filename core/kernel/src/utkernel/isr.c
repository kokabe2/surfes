// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "isr.h"

#include <stdbool.h>
#include <stddef.h>

#include "impl/utkernel.h"
#include "instance_helper.h"

typedef struct IsrStruct {
  int interrupt_number;
} IsrStruct;

static bool Register(Isr self, IsrFunction function) {
  T_DINT packet = {
      .intatr = TA_HLNG,
      .inthdr = (FP)function,
  };
  return tk_def_int((UINT)self->interrupt_number, &packet) == E_OK;
}

Isr Isr_Create(int interrupt_number, IsrFunction function) {
  Isr self = (Isr)InstanceHelper_New(sizeof(IsrStruct));
  if (!self) return NULL;

  self->interrupt_number = interrupt_number;
  if (!Register(self, function)) InstanceHelper_Delete(&self);

  return self;
}

static void Unregister(Isr self) {
  T_DINT packet = {
      .intatr = TA_HLNG,
  };
  tk_def_int((UINT)self->interrupt_number, &packet);
}

void Isr_Destroy(Isr* self) {
  if (!self || !(*self)) return;
  Unregister(*self);
  InstanceHelper_Delete(self);
}

void Isr_Enable(Isr self, int level) {
  if (self) EnableInt((UINT)self->interrupt_number, level);
}

void Isr_Disable(Isr self) {
  if (self) DisableInt((UINT)self->interrupt_number);
}
