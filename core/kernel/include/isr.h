// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_INCLUDE_ISR_H_
#define CORE_KERNEL_INCLUDE_ISR_H_

typedef void (*IsrFunction)(int interrupt_number);
typedef struct IsrStruct* Isr;

Isr Isr_Create(int interrupt_number, IsrFunction function);
void Isr_Destroy(Isr* self);
void Isr_Enable(Isr self, int level);
void Isr_Disable(Isr self);

#endif  // CORE_KERNEL_INCLUDE_ISR_H_
