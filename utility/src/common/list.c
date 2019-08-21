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
  int count;
} ListStruct;

List List_Create(comparator function) {
  List self = (List)InstanceHelper_New(sizeof(ListStruct));
  if (self) {
    self->Compare = function;
    self->tail = &self->head;
  }
  return self;
}

inline static bool IsEmpty(List self) { return self->count == 0; }

inline static ListNode getFirst(List self) { return self->head; }

inline static void UpdateFirst(List self, ListNode node) { self->head = node; }

static ListNode PopFirst(List self) {
  ListNode node = getFirst(self);
  UpdateFirst(self, node->next);
  self->count--;
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

int List_Count(List self) { return self ? self->count : 0; }

void* List_Get(List self, int index) {
  if (!self || (index < 0) || (index >= self->count)) return NULL;

  ListNode node = getFirst(self);
  for (int i = 0; i < index; ++i) node = node->next;
  return node->item;
}

void* List_First(List self) {
  return (self && !IsEmpty(self)) ? getFirst(self)->item : NULL;
}

void* List_Last(List self) {
  return (self && !IsEmpty(self)) ? List_Get(self, self->count - 1) : NULL;
}

static ListNode NewNode(void* item) {
  ListNode node = (ListNode)InstanceHelper_New(sizeof(ListNodeStruct));
  if (node) node->item = item;
  return node;
}

static void AddToTail(List self, ListNode node) {
  *self->tail = node;
  self->tail = &node->next;
  self->count++;
}

void List_Add(List self, void* item) {
  if (!self) return;

  ListNode node = NewNode(item);
  if (node) AddToTail(self, node);
}

void List_Clear(List self) {
  if (self) DeleteAllNodes(self);
}

static bool Equals(List self, void* item, void* match) {
  return self->Compare(item, match) == 0;
}

void* List_Find(List self, void* match) {
  if (!self || !self->Compare || !match) return NULL;

  for (ListNode node = getFirst(self); node; node = node->next)
    if (Equals(self, node->item, match)) return node->item;
  return NULL;
}
