// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_INCLUDE_INBOX_H_
#define CORE_KERNEL_INCLUDE_INBOX_H_

#include <stdbool.h>

enum InboxSize {
  kIsDefaultSize = 1024,
  kIsMaxSize = 4 * 1024,
};

typedef struct InboxStruct* Inbox;

Inbox Inbox_Create(int inbox_size);
void Inbox_Destroy(Inbox* self);
bool Inbox_Post(Inbox self, int message_size, const void* message);
void* Inbox_Get(Inbox self);
bool Inbox_Send(Inbox self, int message_size, const void* message);

#endif  // CORE_KERNEL_INCLUDE_INBOX_H_
