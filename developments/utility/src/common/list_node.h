// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_UTILITY_SRC_COMMON_LIST_NODE_H_
#define DEVELOPMENTS_UTILITY_SRC_COMMON_LIST_NODE_H_

typedef struct ListNodeStruct* ListNode;

ListNode ListNode_Create(void* item);
void ListNode_Destroy(ListNode* self);
void* ListNode_getItem(ListNode self);
ListNode ListNode_getNext(ListNode self);

#endif  // DEVELOPMENTS_UTILITY_SRC_COMMON_LIST_NODE_H_
