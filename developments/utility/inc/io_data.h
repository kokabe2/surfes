// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef UTILITY_INCLUDE_IO_DATA_H_
#define UTILITY_INCLUDE_IO_DATA_H_

#include <stdint.h>

typedef uintptr_t ioAddress;

void IoData_Write8bit(ioAddress offset, uint8_t data);
void IoData_Write16bit(ioAddress offset, uint16_t data);
void IoData_Write32bit(ioAddress offset, uint32_t data);
uint8_t IoData_Read8bit(ioAddress offset);
uint16_t IoData_Read16bit(ioAddress offset);
uint32_t IoData_Read32bit(ioAddress offset);

#endif  // UTILITY_INCLUDE_IO_DATA_H_
