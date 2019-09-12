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

inline static ListNode getFirstNode(List self) { return self->head; }

inline static void setFirstNode(List self, ListNode ln) { self->head = ln; }

static ListNode PopFirstNode(List self) {
  ListNode ln = getFirstNode(self);
  setFirstNode(self, ListNode_getNext(ln));
  self->count--;
  return ln;
}

inline static void DeleteItemIfNeeded(List self, ListNode ln) {
  if (!self->Delete) return;
  void* item = ListNode_getItem(ln);
  self->Delete(&item);
}

void DeleteAllNodes(List self) {
  while (!IsEmpty(self)) {
    ListNode ln = PopFirstNode(self);
    DeleteItemIfNeeded(self, ln);
    ListNode_Destroy(&ln);
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

static ListNode getNode(List self, int index) {
  ListNode ln = getFirstNode(self);
  for (int i = 0; i < index; ++i) ln = ListNode_getNext(ln);
  return ln;
}

void* List_Get(List self, int index) {
  if (!self || !Validate(self, index)) return NULL;

  ListNode ln = getNode(self, index);
  return ListNode_getItem(ln);
}

void* List_getFirst(List self) {
  return (self && !IsEmpty(self)) ? ListNode_getItem(getFirstNode(self)) : NULL;
}

inline static ListNode getLastNode(List self) { return self->tail; }

void* List_getLast(List self) {
  return (self && !IsEmpty(self)) ? ListNode_getItem(getLastNode(self)) : NULL;
}

inline static void setLastNode(List self, ListNode ln) {
  ListNode_setNext(self->tail, ln);
}

static void AddNode(List self, ListNode ln) {
  if (IsEmpty(self))
    setFirstNode(self, ln);
  else
    setLastNode(self, ln);

  self->tail = ln;
  self->count++;
}

void List_Add(List self, void* item) {
  if (!self) return;

  ListNode ln = ListNode_Create(item);
  if (ln) AddNode(self, ln);
}

void List_Clear(List self) {
  if (self) DeleteAllNodes(self);
}

static bool Equals(List self, void* item, void* match) {
  return self->Compare(item, match) == 0;
}

void* List_Find(List self, void* match) {
  if (!self || !self->Compare || !match) return NULL;

  for (ListNode ln = getFirstNode(self); ln; ln = ListNode_getNext(ln))
    if (Equals(self, ListNode_getItem(ln), match)) return ListNode_getItem(ln);
  return NULL;
}

inline static bool IsFirstNode(int index) { return index == 0; }

static ListNode PopNode(List self, int index) {
  if (IsFirstNode(index)) return PopFirstNode(self);

  ListNode pre = getNode(self, index - 1);
  ListNode ln = ListNode_getNext(pre);
  ListNode next = ListNode_getNext(ln);
  ListNode_setNext(pre, next);

  if (!next) self->tail = pre;
  self->count--;
  return ln;
}

static void* PopItem(ListNode ln) {
  void* item = ListNode_getItem(ln);
  ListNode_Destroy(&ln);
  return item;
}

void* List_Pop(List self, int index) {
  if (!self || !Validate(self, index)) return NULL;
  ListNode ln = PopNode(self, index);
  return PopItem(ln);
}
