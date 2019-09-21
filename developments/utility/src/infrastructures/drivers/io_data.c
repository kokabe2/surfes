// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "io_data.h"

void IoData_Write8bit(ioAddress offset, uint8_t data) {
  uint8_t volatile *address = (uint8_t *)offset;
  *address = data;
}

void IoData_Write16bit(ioAddress offset, uint16_t data) {
  uint16_t volatile *address = (uint16_t *)offset;
  *address = data;
}

void IoData_Write32bit(ioAddress offset, uint32_t data) {
  uint32_t volatile *address = (uint32_t *)offset;
  *address = data;
}

void IoData_Add8bit(ioAddress offset, uint8_t data) {
  uint8_t volatile *address = (uint8_t *)offset;
  *address |= data;
}

void IoData_Add16bit(ioAddress offset, uint16_t data) {
  uint16_t volatile *address = (uint16_t *)offset;
  *address |= data;
}

void IoData_Add32bit(ioAddress offset, uint32_t data) {
  uint32_t volatile *address = (uint32_t *)offset;
  *address |= data;
}

void IoData_Remove8bit(ioAddress offset, uint8_t data) {
  uint8_t volatile *address = (uint8_t *)offset;
  *address &= ~data;
}

void IoData_Remove16bit(ioAddress offset, uint16_t data) {
  uint16_t volatile *address = (uint16_t *)offset;
  *address &= ~data;
}

void IoData_Remove32bit(ioAddress offset, uint32_t data) {
  uint32_t volatile *address = (uint32_t *)offset;
  *address &= ~data;
}

uint8_t IoData_Read8bit(ioAddress offset) { return *(uint8_t *)offset; }

uint16_t IoData_Read16bit(ioAddress offset) { return *(uint16_t *)offset; }

uint32_t IoData_Read32bit(ioAddress offset) { return *(uint32_t *)offset; }
