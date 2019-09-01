// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_CORE_BOOT_SRC_DIAGNOSIS_MEMORY_DIAGNOSTIC_TRANSACTION_H_
#define DEVELOPMENTS_CORE_BOOT_SRC_DIAGNOSIS_MEMORY_DIAGNOSTIC_TRANSACTION_H_

#include <stdbool.h>
#include <stdint.h>

void MemoryDiagnosticTransaction_Create(uintptr_t top_address, int memory_size,
                                        int bus_width);
void MemoryDiagnosticTransaction_Destroy(void);
void MemoryDiagnosticTransaction_Add(uint_fast32_t bit_pattern);
bool MemoryDiagnosticTransaction_Execute(void);

#endif  // DEVELOPMENTS_CORE_BOOT_SRC_DIAGNOSIS_MEMORY_DIAGNOSTIC_TRANSACTION_H_
