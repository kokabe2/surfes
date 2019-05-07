// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "ssd_driver.h"

#include <stddef.h>
#include <stdlib.h>

#include "common/runtime_error.h"
#include "led_driver.h"
#include "led_driver_private.h"

enum {
  kNonsenseEncoding = 0,
};

struct SsdDriverStruct {
  LedDriverStruct base;
  uint8_t (*decoder)(uint8_t);
  uint8_t encoding;
};

static bool Validate(uint8_t* io_address, uint8_t (*decoder)(uint8_t)) {
  if (!io_address)
    RUNTIME_ERROR("SSD Driver: null I/O address", 0);
  else if (!decoder)
    RUNTIME_ERROR("SSD Driver: null decode function", 0);
  else
    return true;

  return false;
}

static uint8_t ConvertToBitFrom(int segment_number) {
  return 1 << (segment_number - 1);
}

static SsdDriver NewInstance(uint8_t* io_address, uint8_t (*decoder)(uint8_t)) {
  SsdDriver self = (SsdDriver)calloc(1, sizeof(struct SsdDriverStruct));
  if (self) {
    self->base.io_address = io_address;
    self->base.decoder = ConvertToBitFrom;
    self->decoder = decoder;
  }

  return self;
}

SsdDriver SsdDriver_Create(uint8_t* io_address, uint8_t (*decoder)(uint8_t)) {
  if (!Validate(io_address, decoder)) return NULL;

  SsdDriver self = NewInstance(io_address, decoder);
  if (self) LedDriver_TurnAllOff(&self->base);

  return self;
}

static void DeleteInstance(SsdDriver* self) {
  free(*self);
  *self = NULL;
}

void SsdDriver_Destroy(SsdDriver* self) {
  if (!self || !*self) return;

  LedDriver_TurnAllOff(&(*self)->base);
  DeleteInstance(self);
}

void SsdDriver_TurnOn(SsdDriver self, int segment_number) {
  LedDriver_TurnOn(&self->base, segment_number);
}

void SsdDriver_TurnOff(SsdDriver self, int segment_number) {
  LedDriver_TurnOff(&self->base, segment_number);
}

void SsdDriver_TurnAllOn(SsdDriver self) { LedDriver_TurnAllOn(&self->base); }

void SsdDriver_TurnAllOff(SsdDriver self) { LedDriver_TurnAllOff(&self->base); }

bool SsdDriver_IsOn(SsdDriver self, int segment_number) {
  return LedDriver_IsOn(&self->base, segment_number);
}

bool SsdDriver_IsOff(SsdDriver self, int segment_number) {
  return LedDriver_IsOff(&self->base, segment_number);
}

static bool IsInvalid(SsdDriver self) {
  if (self) return false;

  RUNTIME_ERROR("SSD Driver: null instance", 0);
  return true;
}

static void SetSsdImageBit(SsdDriver self, uint8_t encoding) {
  *self->base.io_address = self->decoder(encoding);
  self->encoding = encoding;
}

void SsdDriver_Set(SsdDriver self, uint8_t encoding) {
  if (IsInvalid(self)) return;

  SetSsdImageBit(self, encoding);
}

static uint8_t GetSsdEncoding(SsdDriver self) { return self->encoding; }

uint8_t SsdDriver_Get(SsdDriver self) {
  if (IsInvalid(self)) return kNonsenseEncoding;

  return GetSsdEncoding(self);
}
