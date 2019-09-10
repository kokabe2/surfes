// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "list.h"

#include <stdbool.h>
#include <stddef.h>

#include "instance_helper.h"
#include "list_node.h"

typedef struct ListStruct {
  ListNode head;
  ListNode tail;
  itemComparator Compare;
  itemDestructor Delete;
  int count;
} ListStruct;

List List_Create(itemComparator ic, itemDestructor id) {
  List self = (List)InstanceHelper_New(sizeof(ListStruct));
  if (self) {
    self->Compare = ic;
    self->Delete = id;
  }
  return self;
}

inline static bool IsEmpty(List self) { return self->count == 0; }

inline static ListNode getFirst(List self) { return self->head; }

inline static void setFirst(List self, ListNode node) { self->head = node; }

static ListNode PopFirst(List self) {
  ListNode node = getFirst(self);
  setFirst(self, ListNode_getNext(node));
  self->count--;
  return node;
}

inline static void DeleteItemIfNeeded(List self, void* item) {
  if (self->Delete) self->Delete(&item);
}

void DeleteAllNodes(List self) {
  while (!IsEmpty(self)) {
    ListNode node = PopFirst(self);
    DeleteItemIfNeeded(self, ListNode_getItem(node));
    ListNode_Destroy(&node);
  }
  self->tail = NULL;
}

void List_Destroy(List* self) {
  if (!self || !*self) return;

  DeleteAllNodes(*self);
  InstanceHelper_Delete(self);
}

int List_Count(List self) { return self ? self->count : 0; }

inline static bool Validate(List self, int index) {
  return (index >= 0) && (index < self->count);
}

void* List_Get(List self, int index) {
  if (!self || !Validate(self, index)) return NULL;

  ListNode node = getFirst(self);
  for (int i = 0; i < index; ++i) node = ListNode_getNext(node);
  return ListNode_getItem(node);
}

void* List_First(List self) {
  return (self && !IsEmpty(self)) ? ListNode_getItem(getFirst(self)) : NULL;
}

void* List_Last(List self) {
  return (self && !IsEmpty(self)) ? List_Get(self, self->count - 1) : NULL;
}

inline static void setLast(List self, ListNode node) {
  ListNode_setNext(self->tail, node);
}

static void AddToTail(List self, ListNode node) {
  if (IsEmpty(self))
    setFirst(self, node);
  else
    setLast(self, node);

  self->tail = node;
  self->count++;
}

void List_Add(List self, void* item) {
  if (!self) return;

  ListNode node = ListNode_Create(item);
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

  for (ListNode node = getFirst(self); node; node = ListNode_getNext(node))
    if (Equals(self, ListNode_getItem(node), match))
      return ListNode_getItem(node);
  return NULL;
}

inline static bool IsLast(List self, int index) {
  return (self->count - 1) == index;
}

static ListNode PopNode(List self, int index) {
  ListNode node = getFirst(self);
  ListNode pre = NULL;
  for (int i = 0; i < index; ++i, node = ListNode_getNext(node)) pre = node;
  ListNode next = ListNode_getNext(node);
  if (pre)
    ListNode_setNext(pre, next);
  else
    setFirst(self, next);
  self->count--;
  return node;
}

static void* PopItem(ListNode node) {
  void* item = ListNode_getItem(node);
  ListNode_Destroy(&node);
  return item;
}

void* List_Pop(List self, int index) {
  if (!self || !Validate(self, index)) return NULL;
  ListNode node = PopNode(self, index);
  return PopItem(node);
}
