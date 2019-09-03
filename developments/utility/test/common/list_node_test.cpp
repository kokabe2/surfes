// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "common/list_node.h"
}

class ListNodeTest : public ::testing::Test {
 protected:
  ListNode instance;
  virtual void SetUp() { instance = NULL; }
  virtual void TearDown() { ListNode_Destroy(&instance); }
};

TEST_F(ListNodeTest, Create) {
  int item = 100;

  instance = ListNode_Create(&item);

  ASSERT_TRUE(instance != NULL);
  EXPECT_EQ(&item, ListNode_getItem(instance));
  EXPECT_EQ(NULL, ListNode_getNext(instance));
}

TEST_F(ListNodeTest, CreateWithNull) {
  instance = ListNode_Create(NULL);

  ASSERT_TRUE(instance != NULL);
  EXPECT_EQ(NULL, ListNode_getItem(instance));
  EXPECT_EQ(NULL, ListNode_getNext(instance));
}

TEST_F(ListNodeTest, GetItemWithNull) {
  EXPECT_EQ(NULL, ListNode_getItem(NULL));
}

TEST_F(ListNodeTest, GetNextWithNull) {
  EXPECT_EQ(NULL, ListNode_getNext(NULL));
}
