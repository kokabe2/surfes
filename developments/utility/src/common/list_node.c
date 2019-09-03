// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "list_node.h"

#include <stddef.h>

#include "instance_helper.h"

typedef struct ListNodeStruct {
  void* item;
  ListNode next;
} ListNodeStruct;

ListNode ListNode_Create(void* item) {
  ListNode self = (ListNode)InstanceHelper_New(sizeof(ListNodeStruct));
  if (self) self->item = item;
  return self;
}

void ListNode_Destroy(ListNode* self) { InstanceHelper_Delete(self); }

void* ListNode_getItem(ListNode self) { return self ? self->item : NULL; }

void ListNode_setItem(ListNode self, void* item) {
  if (self) self->item = item;
}

ListNode ListNode_getNext(ListNode self) { return self ? self->next : NULL; }

void ListNode_setNext(ListNode self, ListNode next) {
  if (self) self->next = next;
}
