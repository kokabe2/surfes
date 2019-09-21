// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "ssd_driver.h"

#include <stddef.h>

#include "instance_helper.h"
#include "led_driver_private.h"

enum {
  kNonsenseEncoding = 0,
};

typedef struct SsdDriverStruct {
  LedDriverStruct base;
  ssdDecoder decoder;
  char encoding;
} SsdDriverStruct, *SsdDriver;

inline static bool Validate(uint8_t* io_address, ssdDecoder decoder) {
  return io_address && decoder;
}

inline static uint8_t ConvertToBit(int segment_number) {
  return 1 << (segment_number - 1);
}

static LedDriver NewInstance(uint8_t* io_address, ssdDecoder decoder) {
  SsdDriver self = (SsdDriver)InstanceHelper_New(sizeof(SsdDriverStruct));
  if (self) {
    self->base.io_address = io_address;
    self->base.decoder = ConvertToBit;
    self->decoder = decoder;
  }
  return (LedDriver)self;
}

LedDriver SsdDriver_Create(uint8_t* io_address, ssdDecoder decoder) {
  if (!Validate(io_address, decoder)) return NULL;

  LedDriver self = NewInstance(io_address, decoder);
  if (self) LedDriver_TurnAllOff(self);
  return self;
}

inline static SsdDriver downcast(LedDriver self) { return (SsdDriver)self; }

inline static void SetSsdImageBit(SsdDriver self, char encoding) {
  *self->base.io_address = self->decoder(encoding);
  self->encoding = encoding;
}

void SsdDriver_Set(LedDriver self, char encoding) {
  if (self) SetSsdImageBit(downcast(self), encoding);
}

char SsdDriver_Get(LedDriver self) {
  return self ? downcast(self)->encoding : kNonsenseEncoding;
}
