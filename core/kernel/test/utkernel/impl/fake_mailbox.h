// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_MAILBOX_H_
#define CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_MAILBOX_H_

#include <stdbool.h>

#include "utkernel.h"

void fake_mailbox_init(void);
ATR fake_mailbox_getAttribute(ID mbxid);
void* fake_mailbox_getLastMail(ID mbxid);
bool fake_mailbox_isCreated(ID mbxid);

#endif  // CORE_KERNEL_TEST_UTKERNEL_IMPL_FAKE_MAILBOX_H_
