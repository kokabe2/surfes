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

static bool Register(int interrupt_number, IsrFunction function) {
  T_DINT packet = {
      .intatr = TA_HLNG,
      .inthdr = (FP)function,
  };
  return tk_def_int((UINT)interrupt_number, &packet) == E_OK;
}

static Isr NewInstance(int interrupt_number) {
  Isr self = (Isr)InstanceHelper_New(sizeof(IsrStruct));
  if (self) self->interrupt_number = interrupt_number;
  return self;
}

static void Unregister(int interrupt_number) {
  T_DINT packet = {
      .intatr = TA_HLNG,
  };
  tk_def_int((UINT)interrupt_number, &packet);
}

Isr Isr_Create(int interrupt_number, IsrFunction function) {
  if (!Register(interrupt_number, function)) return NULL;
  Isr self = NewInstance(interrupt_number);
  if (!self) Unregister(interrupt_number);
  return self;
}

void Isr_Destroy(Isr* self) {
  if (!self || !(*self)) return;
  Unregister((*self)->interrupt_number);
  InstanceHelper_Delete(self);
}

void Isr_Enable(Isr self, int level) {
  if (self) EnableInt((UINT)self->interrupt_number, level);
}

void Isr_Disable(Isr self) {
  if (self) DisableInt((UINT)self->interrupt_number);
}
