﻿// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "list.h"
}

namespace {
int CompareInteger(void* v1, void* v2) {
  int* i1 = (int*)v1;
  int* i2 = (int*)v2;
  return *i1 == *i2;
}
}  // namespace

class ListTest : public ::testing::Test {
 protected:
  List instance;

  virtual void SetUp() { instance = List_Create(CompareInteger); }
  virtual void TearDown() { List_Destroy(&instance); }
};

TEST_F(ListTest, Create) {
  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ(0, List_Count(instance));
  EXPECT_EQ(NULL, List_Get(instance, 0));
  EXPECT_EQ(NULL, List_First(instance));
  EXPECT_EQ(NULL, List_Last(instance));
}

TEST_F(ListTest, Destroy) {
  List_Destroy(&instance);

  EXPECT_EQ(NULL, instance);
}

TEST_F(ListTest, DestroyMutipleTimes) {
  List_Destroy(&instance);
  List_Destroy(&instance);

  SUCCEED();
}

TEST_F(ListTest, DestroyWithNull) {
  List_Destroy(NULL);

  SUCCEED();
}

TEST_F(ListTest, AddOneItem) {
  int item = 1234;

  List_Add(instance, &item);

  EXPECT_EQ(1, List_Count(instance));
  EXPECT_EQ(&item, List_Get(instance, 0));
  EXPECT_EQ(&item, List_First(instance));
  EXPECT_EQ(&item, List_Last(instance));
}

TEST_F(ListTest, AddMultipleItems) {
  int item0 = -42;
  int item1 = 256;
  int item2 = 3141;

  List_Add(instance, &item0);
  List_Add(instance, &item1);
  List_Add(instance, &item2);

  EXPECT_EQ(3, List_Count(instance));
  EXPECT_EQ(&item1, List_Get(instance, 1));
  EXPECT_EQ(&item0, List_First(instance));
  EXPECT_EQ(&item2, List_Last(instance));
}

TEST_F(ListTest, AddWithNull) {
  int item = 1234;

  List_Add(NULL, &item);

  SUCCEED();
}

TEST_F(ListTest, CountWithNull) { EXPECT_EQ(0, List_Count(NULL)); }

TEST_F(ListTest, GetWithNull) { EXPECT_EQ(NULL, List_Get(NULL, 0)); }

TEST_F(ListTest, GetWithIndexMoreThanAdded) {
  int item = 1234;

  List_Add(instance, &item);

  EXPECT_EQ(NULL, List_Get(NULL, 1));
  EXPECT_EQ(NULL, List_Get(NULL, 124));
}

TEST_F(ListTest, GetWithIndexLessThanZero) {
  EXPECT_EQ(NULL, List_Get(instance, -1));
  EXPECT_EQ(NULL, List_Get(instance, -243));
}

TEST_F(ListTest, FirstWithNull) { EXPECT_EQ(NULL, List_First(NULL)); }

TEST_F(ListTest, LastWithNull) { EXPECT_EQ(NULL, List_Last(NULL)); }

TEST_F(ListTest, Clear) {
  List_Add(instance, NULL);
  List_Add(instance, NULL);
  EXPECT_EQ(2, List_Count(instance));

  List_Clear(instance);

  EXPECT_EQ(0, List_Count(instance));
}

TEST_F(ListTest, ClearWithNull) {
  List_Clear(NULL);

  SUCCEED();
}
