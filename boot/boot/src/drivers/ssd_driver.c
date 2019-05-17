// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "ssd_driver.h"

#include <stddef.h>
#include <stdlib.h>

#include "led_driver.h"
#include "led_driver_private.h"
#include "utilities/instance_helper.h"
#include "utilities/runtime_error.h"

enum {
  kNonsenseEncoding = 0,
};

typedef struct SsdDriverStruct {
  LedDriverStruct base;
  ssdDecoder decoder;
  char encoding;
} SsdDriverStruct;

static bool IsInvalidAddress(uint8_t* io_address) {
  if (io_address) return false;

  RUNTIME_ERROR("SSD Driver: null I/O address", 0);
  return true;
}

static bool IsInvalidDecoder(ssdDecoder decoder) {
  if (decoder) return false;

  RUNTIME_ERROR("SSD Driver: null decode function", 0);
  return true;
}

static uint8_t ConvertToBitFrom(int segment_number) {
  return 1 << (segment_number - 1);
}

static SsdDriver NewInstance(uint8_t* io_address, ssdDecoder decoder) {
  SsdDriver self = (SsdDriver)InstanceHelper_New(sizeof(SsdDriverStruct));
  if (self) {
    self->base.io_address = io_address;
    self->base.decoder = ConvertToBitFrom;
    self->decoder = decoder;
  }

  return self;
}

SsdDriver SsdDriver_Create(uint8_t* io_address, ssdDecoder decoder) {
  if (IsInvalidAddress(io_address) || IsInvalidDecoder(decoder)) return NULL;

  SsdDriver self = NewInstance(io_address, decoder);
  if (self) LedDriver_TurnAllOff(&self->base);

  return self;
}

void SsdDriver_Destroy(SsdDriver* self) {
  if (!self || !*self) return;

  LedDriver_TurnAllOff(&(*self)->base);
  InstanceHelper_Delete(self);
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

static void SetSsdImageBit(SsdDriver self, char encoding) {
  *self->base.io_address = self->decoder(encoding);
  self->encoding = encoding;
}

void SsdDriver_Set(SsdDriver self, char encoding) {
  if (IsInvalid(self)) return;

  SetSsdImageBit(self, encoding);
}

static char GetSsdEncoding(SsdDriver self) { return self->encoding; }

char SsdDriver_Get(SsdDriver self) {
  if (IsInvalid(self)) return kNonsenseEncoding;

  return GetSsdEncoding(self);
}
