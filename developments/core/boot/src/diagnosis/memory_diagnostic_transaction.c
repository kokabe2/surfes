// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "memory_diagnostic_transaction.h"

#include <stddef.h>

#include "instance_helper.h"
#include "list.h"
#include "memory_diagnostician_factory.h"

static uintptr_t its_top_address;
static int its_memory_size;
static int its_bus_width;
static List its_bit_patterns = NULL;

static uint_fast32_t* NewBitPattern(uint_fast32_t bit_pattern) {
  uint_fast32_t* self =
      (uint_fast32_t*)InstanceHelper_New(sizeof(uint_fast32_t));
  if (self) *self = bit_pattern;
  return self;
}

inline static void DeleteBitPattern(uint_fast32_t* self) {
  InstanceHelper_Delete(&self);
}

inline static bool Created(void) { return its_bit_patterns != NULL; }

static void DeleteBitPatterns(void) {
  uint_fast32_t* bp;
  while ((bp = (uint_fast32_t*)List_Pop(its_bit_patterns, 0)) != NULL)
    DeleteBitPattern(bp);
  List_Destroy(&its_bit_patterns);
}

static void NewBitPatterns(void) {
  if (Created()) DeleteBitPatterns();
  its_bit_patterns = List_Create(NULL, NULL);
}

void MemoryDiagnosticTransaction_Create(uintptr_t top_address, int memory_size,
                                        int bus_width) {
  its_top_address = top_address;
  its_memory_size = memory_size;
  its_bus_width = bus_width;
  NewBitPatterns();
}

void MemoryDiagnosticTransaction_Destroy(void) { DeleteBitPatterns(); }

void MemoryDiagnosticTransaction_Add(uint_fast32_t bit_pattern) {
  if (!Created()) return;

  uint_fast32_t* bp = NewBitPattern(bit_pattern);
  if (bp) List_Add(its_bit_patterns, bp);
}

bool MemoryDiagnosticTransaction_Execute(void) {
  if (!Created()) return false;

  IMemoryDiagnosable imd = MemoryDiagnosticianFactory_Make(its_bus_width);
  int count = List_Count(its_bit_patterns);
  for (int i = 0; i < count; ++i) {
    uint_fast32_t* bp = (uint_fast32_t*)List_Get(its_bit_patterns, i);
    if (!imd->ReadAfterWrite(its_top_address, its_memory_size, *bp))
      return false;
  }
  return true;
}
