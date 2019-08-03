// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "heap.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "impl/utkernel.h"

enum {
  kNonsenseId = ~0,
};

static struct {
  int id;
} the_instance = {
    kNonsenseId,
};

static bool Validate(int heap_size, void* heap) {
  return heap_size > 0 && heap;
}

static bool Created(void) { return the_instance.id != kNonsenseId; }

static void Create(int heap_size, void* heap) {
  T_CMPL packet = {
      .mplatr = (TA_TFIFO | TA_RNG0 | TA_USERBUF),
      .mplsz = heap_size,
      .bufptr = heap,
  };
  the_instance.id = tk_cre_mpl(&packet);
}

void Heap_Create(int heap_size, void* heap) {
  if (!Validate(heap_size, heap)) return;
  if (Created()) Heap_Destroy();
  Create(heap_size, heap);
}

void Heap_Destroy(void) {
  tk_del_mpl(the_instance.id);
  the_instance.id = kNonsenseId;
}

static bool IsValid(int memory_size) { return memory_size > 0; }

static void* Allocate(int memory_size) {
  void* memory;
  return tk_get_mpl(the_instance.id, memory_size, &memory, TMO_POL) == E_OK
             ? memset(memory, 0, memory_size)
             : NULL;
}

void* Heap_Allocate(int memory_size) {
  return IsValid(memory_size) ? Allocate(memory_size) : NULL;
}

void Heap_Release(void* memory) {
  if (memory) tk_rel_mpl(the_instance.id, memory);
}
