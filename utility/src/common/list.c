// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "list.h"

#include "stddef.h"

#include "instance_helper.h"

typedef struct ListNodeStruct* ListNode;
typedef struct ListNodeStruct {
  void* item;
  ListNode next;
} ListNodeStruct;

typedef struct ListStruct {
  ListNode head;
  comparator Compare;
} ListStruct;

static ListNode NewNode(void* item) {
  ListNode node = (ListNode)InstanceHelper_New(sizeof(ListNodeStruct));
  if (node) node->item = item;
  return node;
}

List List_Create(comparator function) {
  List self = (List)InstanceHelper_New(sizeof(ListStruct));
  if (self) self->Compare = function;
  return self;
}

static bool IsEmpty(List self) { return self->head == NULL; }

static ListNode FirstNode(List self) { return self->head; }

void DeleteAllNodes(List self) {
  ListNode node = FirstNode(self);
  do {
    ListNode next = node->next;
    InstanceHelper_Delete(&node);
    node = next;
  } while (node);
}

void List_Destroy(List* self) {
  if (!self || !*self) return;

  if (!IsEmpty(*self)) DeleteAllNodes(*self);
  InstanceHelper_Delete(self);
}

int List_Count(List self) {
  if (!self) return 0;

  int count = 0;
  for (ListNode node = FirstNode(self); node; node = node->next) ++count;
  return count;
}

void* List_Get(List self, int index) {
  if (!self || (index < 0)) return NULL;

  for (ListNode node = FirstNode(self); node; node = node->next, --index)
    if (index == 0) return node->item;
  return NULL;
}

static ListNode LastNode(List self) {
  for (ListNode node = FirstNode(self); node; node = node->next)
    if (node->next == NULL) return node;
  return NULL;
}

void* List_First(List self) {
  if (!self) return NULL;
  return IsEmpty(self) ? NULL : FirstNode(self)->item;
}

void* List_Last(List self) {
  if (!self) return NULL;
  return IsEmpty(self) ? NULL : LastNode(self)->item;
}

static void AddFirst(List self, ListNode node) { self->head = node; }

static void AddLast(List self, ListNode node) { LastNode(self)->next = node; }

void List_Add(List self, void* item) {
  if (!self) return;

  ListNode node = NewNode(item);
  if (IsEmpty(self))
    AddFirst(self, node);
  else
    AddLast(self, node);
}
