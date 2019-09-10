// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_UTILITY_INC_LIST_H_
#define DEVELOPMENTS_UTILITY_INC_LIST_H_

typedef int (*itemComparator)(void*, void*);
typedef void (*itemDestructor)(void**);
typedef struct ListStruct* List;

List List_Create(itemComparator ic, itemDestructor id);
void List_Destroy(List* self);
int List_Count(List self);
void* List_Get(List self, int index);
void* List_getFirst(List self);
void* List_Last(List self);
void List_Add(List self, void* item);
void List_Clear(List self);
void* List_Find(List self, void* match);
void* List_Pop(List self, int index);
// void List_Extend(List self, List items);
// void List_Insert(List self, int index,  void* item);
// void List_Remove(List self,  void* item);
// void List_Reverse(List self);
// void List_Sort(List self);

#endif  // DEVELOPMENTS_UTILITY_INC_LIST_H_
