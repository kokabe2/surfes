// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "list.h"

#include <stdbool.h>
#include <stddef.h>

#include "instance_helper.h"

typedef struct ListNodeStruct* ListNode;
typedef struct ListNodeStruct {
  void* item;
  ListNode next;
} ListNodeStruct;

typedef struct ListStruct {
  ListNode head;
  comparator Compare;
  ListNode* tail;
} ListStruct;

List List_Create(comparator function) {
  List self = (List)InstanceHelper_New(sizeof(ListStruct));
  if (self) {
    self->Compare = function;
    self->tail = &self->head;
  }
  return self;
}

inline static bool IsEmpty(List self) { return self->head == NULL; }

inline static ListNode getFirst(List self) { return self->head; }

inline static void UpdateFirst(List self, ListNode node) { self->head = node; }

static ListNode PopFirst(List self) {
  ListNode node = getFirst(self);
  UpdateFirst(self, node->next);
  return node;
}

void DeleteAllNodes(List self) {
  while (!IsEmpty(self)) {
    ListNode node = PopFirst(self);
    InstanceHelper_Delete(&node);
  }
}

void List_Destroy(List* self) {
  if (!self || !*self) return;

  DeleteAllNodes(*self);
  InstanceHelper_Delete(self);
}

int List_Count(List self) {
  if (!self) return 0;

  int count = 0;
  for (ListNode node = getFirst(self); node; node = node->next) ++count;
  return count;
}

void* List_Get(List self, int index) {
  if (!self || (index < 0)) return NULL;

  for (ListNode node = getFirst(self); node; node = node->next, --index)
    if (index == 0) return node->item;
  return NULL;
}

static ListNode getLast(List self) {
  for (ListNode node = getFirst(self); node; node = node->next)
    if (node->next == NULL) return node;
  return NULL;
}

void* List_First(List self) {
  return (self && !IsEmpty(self)) ? getFirst(self)->item : NULL;
}

void* List_Last(List self) {
  return (self && !IsEmpty(self)) ? getLast(self)->item : NULL;
}

static ListNode NewNode(void* item) {
  ListNode node = (ListNode)InstanceHelper_New(sizeof(ListNodeStruct));
  if (node) node->item = item;
  return node;
}

static void AddToTail(List self, ListNode node) {
  *self->tail = node;
  self->tail = &node->next;
}

void List_Add(List self, void* item) {
  if (!self) return;

  ListNode node = NewNode(item);
  if (node) AddToTail(self, node);
}

void List_Clear(List self) {
  if (self) DeleteAllNodes(self);
}
