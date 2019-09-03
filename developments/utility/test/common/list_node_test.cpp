// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "common/list_node.h"
}

class ListNodeTest : public ::testing::Test {
 protected:
  int item;
  ListNode item_node;
  ListNode null_node;

  virtual void SetUp() {
    item_node = ListNode_Create(&item);
    null_node = ListNode_Create(NULL);
  }

  virtual void TearDown() { ListNode_Destroy(&item_node); }
};

TEST_F(ListNodeTest, Create) {
  ASSERT_TRUE(item_node != NULL);
  EXPECT_EQ(&item, ListNode_getItem(item_node));
  EXPECT_EQ(NULL, ListNode_getNext(item_node));
}

TEST_F(ListNodeTest, CreateWithNull) {
  ASSERT_TRUE(null_node != NULL);
  EXPECT_EQ(NULL, ListNode_getItem(null_node));
  EXPECT_EQ(NULL, ListNode_getNext(null_node));
}

TEST_F(ListNodeTest, GetItemWithNull) {
  EXPECT_EQ(NULL, ListNode_getItem(NULL));
}

TEST_F(ListNodeTest, SetItem) {
  ListNode_setItem(null_node, &item);

  EXPECT_EQ(&item, ListNode_getItem(null_node));
}

TEST_F(ListNodeTest, SetItemWithNullInstance) {
  ListNode_setItem(NULL, &item);

  SUCCEED();
}

TEST_F(ListNodeTest, SetItemWithNullItem) {
  ListNode_setItem(item_node, NULL);

  EXPECT_EQ(NULL, ListNode_getItem(item_node));
}

TEST_F(ListNodeTest, GetNextWithNull) {
  EXPECT_EQ(NULL, ListNode_getNext(NULL));
}

TEST_F(ListNodeTest, SetNext) {
  ListNode_setNext(item_node, null_node);

  EXPECT_EQ(null_node, ListNode_getNext(item_node));
}

TEST_F(ListNodeTest, SetNextWithNullInstance) {
  ListNode_setNext(NULL, item_node);

  SUCCEED();
}

TEST_F(ListNodeTest, SetNextWithNullNextNode) {
  ListNode_setNext(item_node, null_node);

  ListNode_setNext(item_node, NULL);

  EXPECT_EQ(NULL, ListNode_getNext(item_node));
}
