// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "memory_diagnostician_factory.h"

#include <limits.h>

enum {
  kNonsenseBusWidth = 0,
};

inline static bool Validate(uintptr_t top_address, int size) {
  return (size >= 0) && (top_address <= (UINTPTR_MAX - size));
}

static bool ReadAfterWriteIn8Bit(uintptr_t top_address, int size,
                                 int bit_pattern) {
  if (!Validate(top_address, size) || (bit_pattern > UINT8_MAX)) return false;

  uint8_t* address = (uint8_t*)top_address;
  int count = size;
  for (int i = 0; i < count; ++i) address[i] = bit_pattern;
  for (int i = 0; i < count; ++i)
    if (address[i] != bit_pattern) return false;
  return true;
}

static bool ReadAfterWriteIn16Bit(uintptr_t top_address, int size,
                                  int bit_pattern) {
  if (!Validate(top_address, size) || (bit_pattern > UINT16_MAX)) return false;

  uint16_t* address = (uint16_t*)top_address;
  int count = size / 2;
  for (int i = 0; i < count; ++i) address[i] = bit_pattern;
  for (int i = 0; i < count; ++i)
    if (address[i] != bit_pattern) return false;
  return true;
}

static bool ReadAfterWriteIn32Bit(uintptr_t top_address, int size,
                                  int bit_pattern) {
  if (!Validate(top_address, size) || (bit_pattern > UINT32_MAX)) return false;

  uint32_t* address = (uint32_t*)top_address;
  int count = size / 4;
  for (int i = 0; i < count; ++i) address[i] = bit_pattern;
  for (int i = 0; i < count; ++i)
    if (address[i] != bit_pattern) return false;
  return true;
}

static bool DummyReadAfterWrite(uintptr_t a, int b, int c) { return false; }

typedef struct {
  int bus_width;
  IMemoryDiagnosableStruct diagnostician;
} MemoryDiagnosticianSetStruct, *MemoryDiagnosticianSet;

static const MemoryDiagnosticianSetStruct kDiagnosticians[] = {
    {1, {.ReadAfterWrite = ReadAfterWriteIn8Bit}},
    {2, {.ReadAfterWrite = ReadAfterWriteIn16Bit}},
    {4, {.ReadAfterWrite = ReadAfterWriteIn32Bit}},
    {kNonsenseBusWidth, {.ReadAfterWrite = DummyReadAfterWrite}},
};

static IMemoryDiagnosable RetrieveDiagnostician(int bus_width) {
  MemoryDiagnosticianSet mds = (MemoryDiagnosticianSet)kDiagnosticians;
  for (; mds->bus_width != kNonsenseBusWidth; ++mds)
    if (mds->bus_width == bus_width) return &mds->diagnostician;
  return &mds->diagnostician;  // = DummyReadAfterWrite
}

IMemoryDiagnosable MemoryDiagnosticianFactory_Make(int bus_width) {
  return RetrieveDiagnostician(bus_width);
}
