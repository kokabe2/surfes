// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_CORE_BOOT_SRC_DIAGNOSIS_MEMORY_DIAGNOSTICIAN_FACTORY_H_
#define DEVELOPMENTS_CORE_BOOT_SRC_DIAGNOSIS_MEMORY_DIAGNOSTICIAN_FACTORY_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  bool (*ReadAfterWrite)(uintptr_t top_address, int size, int bit_pattern);
} IMemoryDiagnosableStruct, *IMemoryDiagnosable;

IMemoryDiagnosable MemoryDiagnosticianFactory_Make(int bus_width);

#endif  // DEVELOPMENTS_CORE_BOOT_SRC_DIAGNOSIS_MEMORY_DIAGNOSTICIAN_FACTORY_H_
