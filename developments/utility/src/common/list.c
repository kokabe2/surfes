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
  itemComparator Compare;
  itemDestructor Delete;
  ListNode* tail;
  int count;
} ListStruct;

inline static void ResetTail(List self) { self->tail = &self->head; }

List List_Create(itemComparator ic, itemDestructor id) {
  List self = (List)InstanceHelper_New(sizeof(ListStruct));
  if (self) {
    self->Compare = ic;
    self->Delete = id;
    ResetTail(self);
  }
  return self;
}

inline static bool IsEmpty(List self) { return self->count == 0; }

inline static ListNode getFirst(List self) { return self->head; }

inline static void setFirst(List self, ListNode node) { self->head = node; }

static ListNode PopFirst(List self) {
  ListNode node = getFirst(self);
  setFirst(self, node->next);
  self->count--;
  return node;
}

inline static void DeleteItemIfNeeded(List self, void* item) {
  if (self->Delete) self->Delete(&item);
}

inline static void DeleteNode(ListNode* node) { InstanceHelper_Delete(node); }

void DeleteAllNodes(List self) {
  while (!IsEmpty(self)) {
    ListNode node = PopFirst(self);
    DeleteItemIfNeeded(self, node->item);
    DeleteNode(&node);
  }
  ResetTail(self);
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

inline static bool IsLast(List self, int index) {
  return (self->count - 1) == index;
}

static ListNode PopNode(List self, int index) {
  ListNode node = getFirst(self);
  ListNode* next = &self->head;
  for (int i = 0; i < index; ++i, node = node->next) next = &node->next;
  *next = node->next;
  if (IsLast(self, index)) self->tail = &node->next;
  self->count--;
  return node;
}

static void* PopItem(ListNode node) {
  void* item = node->item;
  DeleteNode(&node);
  return item;
}

void* List_Pop(List self, int index) {
  if (!self || (index < 0) || (index >= self->count)) return NULL;
  ListNode node = PopNode(self, index);
  return PopItem(node);
}
