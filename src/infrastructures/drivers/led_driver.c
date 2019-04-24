// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "led_driver.h"
#include "led_driver_private.h"

#include <stddef.h>
#include <stdlib.h>

#include "common/runtime_error.h"

enum {
  kFirstLed = 1,
  kLastLed = 8,
};

static bool Validate(uint8_t* io_address, uint8_t (*decoder)(int)) {
  if (!io_address)
    RUNTINE_ERROR("LED Driver: null I/O address", 0);
  else if (!decoder)
    RUNTINE_ERROR("LED Driver: null decode function", 0);
  else
    return true;

  return false;
}

static LedDriver NewInstance(uint8_t* io_address, uint8_t (*decoder)(int)) {
  LedDriver self = (LedDriver)calloc(1, sizeof(LedDriverStruct));
  if (self) {
    self->io_address = io_address;
    self->decoder = decoder;
  }

  return self;
}

LedDriver LedDriver_Create(uint8_t* io_address, uint8_t (*decoder)(int)) {
  if (!Validate(io_address, decoder)) return NULL;

  LedDriver self = NewInstance(io_address, decoder);
  if (self) LedDriver_TurnAllOff(self);

  return self;
}

static void DeleteInstance(LedDriver* self) {
  free(*self);
  *self = NULL;
}

void LedDriver_Destroy(LedDriver* self) {
  if (!self || !*self) return;

  LedDriver_TurnAllOff(*self);
  DeleteInstance(self);
}

static bool IsInvalid(LedDriver self) {
  if (self) return false;

  RUNTINE_ERROR("LED Driver: null instance", 0);
  return true;
}

static bool IsLedOutOfBounds(int led_number) {
  if (led_number >= kFirstLed && led_number <= kLastLed) return false;

  RUNTINE_ERROR("LED Driver: out-of-bounds LED", led_number);
  return true;
}

static void SetLedImageBit(LedDriver self, int led_number) {
  *self->io_address |= self->decoder(led_number);
}

void LedDriver_TurnOn(LedDriver self, int led_number) {
  if (IsInvalid(self) || IsLedOutOfBounds(led_number)) return;

  SetLedImageBit(self, led_number);
}

static void ClearLedImageBit(LedDriver self, int led_number) {
  *self->io_address &= ~self->decoder(led_number);
}

void LedDriver_TurnOff(LedDriver self, int led_number) {
  if (IsInvalid(self) || IsLedOutOfBounds(led_number)) return;

  ClearLedImageBit(self, led_number);
}

static void SetAllLedImageBits(LedDriver self) {
  for (int i = kFirstLed; i <= kLastLed; i++)
    *self->io_address |= self->decoder(i);
}

void LedDriver_TurnAllOn(LedDriver self) {
  if (IsInvalid(self)) return;

  SetAllLedImageBits(self);
}

static void ClearAllLedImageBits(LedDriver self) {
  for (int i = kFirstLed; i <= kLastLed; i++)
    *self->io_address &= ~self->decoder(i);
}

void LedDriver_TurnAllOff(LedDriver self) {
  if (IsInvalid(self)) return;

  ClearAllLedImageBits(self);
}

static bool IsLedImageBitOn(LedDriver self, int led_number) {
  return *self->io_address & self->decoder(led_number);
}

bool LedDriver_IsOn(LedDriver self, int led_number) {
  if (IsInvalid(self) || IsLedOutOfBounds(led_number)) return false;

  return IsLedImageBitOn(self, led_number);
}

bool LedDriver_IsOff(LedDriver self, int led_number) {
  return !LedDriver_IsOn(self, led_number);
}
