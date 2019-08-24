// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_MEMORYPOOL_H_
#define CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_MEMORYPOOL_H_

#include <stdbool.h>

#include "utkernel.h"

void fake_memorypool_init(void);
ATR fake_memorypool_getAttribute(ID mplid);
SZ fake_memorypool_getSize(ID mplid);
SZ fake_memorypool_getUsedSize(ID mplid);
bool fake_memorypool_isCreated(ID mplid);

#endif  // CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_MEMORYPOOL_H_
