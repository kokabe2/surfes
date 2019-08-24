// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_INCLUDE_HEAP_H_
#define CORE_KERNEL_INCLUDE_HEAP_H_

void Heap_Create(int heap_size, void* heap);
void Heap_Destroy(void);
void* Heap_Allocate(int memory_size);
void Heap_Release(void* memory);

#endif  // CORE_KERNEL_INCLUDE_HEAP_H_
