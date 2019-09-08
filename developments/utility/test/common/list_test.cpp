// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "list.h"
}

namespace {
int CompareInteger(void* v1, void* v2) {
  int* i1 = (int*)v1;
  int* i2 = (int*)v2;
  return *i1 - *i2;
}

int destruction_count;
void* last_item;
void DummyDelete(void** self) {
  last_item = *self;
  destruction_count++;
}
}  // namespace

class ListTest : public ::testing::Test {
 protected:
  List instance;
  virtual void SetUp() {
    destruction_count = 0;
    last_item = NULL;
    instance = List_Create(CompareInteger, DummyDelete);
  }
  virtual void TearDown() { List_Destroy(&instance); }
};

TEST_F(ListTest, Create) {
  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ(0, List_Count(instance));
  EXPECT_EQ(NULL, List_Get(instance, 0));
  EXPECT_EQ(NULL, List_First(instance));
  EXPECT_EQ(NULL, List_Last(instance));
}

TEST_F(ListTest, CreateWithNullComparator) {
  List l = List_Create(NULL, DummyDelete);
  EXPECT_EQ(0, List_Count(instance));
  EXPECT_EQ(NULL, List_Get(instance, 0));
  EXPECT_EQ(NULL, List_First(instance));
  EXPECT_EQ(NULL, List_Last(instance));
  List_Destroy(&l);
}

TEST_F(ListTest, CreateWithNullDestructor) {
  List l = List_Create(CompareInteger, NULL);
  EXPECT_EQ(0, List_Count(instance));
  EXPECT_EQ(NULL, List_Get(instance, 0));
  EXPECT_EQ(NULL, List_First(instance));
  EXPECT_EQ(NULL, List_Last(instance));
  List_Destroy(&l);
}

TEST_F(ListTest, Destroy) {
  int item = 0;
  List_Add(instance, NULL);
  List_Add(instance, NULL);
  List_Add(instance, &item);

  List_Destroy(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_EQ(3, destruction_count);
  EXPECT_EQ(&item, last_item);
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

TEST_F(ListTest, ClearThenAdd) {
  List_Add(instance, NULL);
  List_Clear(instance);

  int item0 = 1;
  int item1 = -1;
  List_Add(instance, &item0);
  List_Add(instance, &item1);

  EXPECT_EQ(2, List_Count(instance));
  EXPECT_EQ(&item0, List_Get(instance, 0));
  EXPECT_EQ(&item0, List_First(instance));
  EXPECT_EQ(&item1, List_Last(instance));
}

TEST_F(ListTest, ClearWithNull) {
  List_Clear(NULL);

  SUCCEED();
}

TEST_F(ListTest, Find) {
  int item0 = 128;
  int item1 = -6;
  List_Add(instance, &item0);
  List_Add(instance, &item1);
  int match = item1;

  EXPECT_EQ(&item1, (int*)List_Find(instance, &match));
}

TEST_F(ListTest, FindWithNotAddedItem) {
  int match = 0;

  EXPECT_EQ(NULL, (int*)List_Find(instance, &match));
}

TEST_F(ListTest, FindWithNullInstance) {
  int match;

  EXPECT_EQ(NULL, List_Find(NULL, &match));
}

TEST_F(ListTest, FindWithNullItem) {
  EXPECT_EQ(NULL, List_Find(instance, NULL));
}

TEST_F(ListTest, FindIfComparatorIsNotSet) {
  List list = List_Create(NULL, NULL);
  int item = 256;
  List_Add(list, &item);

  EXPECT_EQ(NULL, List_Find(list, &item));
  List_Destroy(&list);
}

TEST_F(ListTest, PopFromListHasNoItem) {
  EXPECT_EQ(NULL, List_Pop(instance, 0));
  EXPECT_EQ(0, List_Count(instance));
}

TEST_F(ListTest, PopFromListHasOneItem) {
  int item = 1;
  List_Add(instance, &item);

  EXPECT_EQ(&item, List_Pop(instance, 0));
  EXPECT_EQ(0, List_Count(instance));
}

TEST_F(ListTest, Pop) {
  int item0 = -42;
  int item1 = 256;
  int item2 = 3141;
  List_Add(instance, &item0);
  List_Add(instance, &item1);
  List_Add(instance, &item2);

  EXPECT_EQ(&item1, List_Pop(instance, 1));
  EXPECT_EQ(&item2, List_Pop(instance, 1));
  EXPECT_EQ(1, List_Count(instance));
}

TEST_F(ListTest, PopWithNull) { EXPECT_EQ(NULL, List_Pop(NULL, 0)); }

TEST_F(ListTest, PopWithIndexLessThanZero) {
  EXPECT_EQ(NULL, List_Pop(instance, -1));
}

TEST_F(ListTest, PopWithIndexMoreThanAdded) {
  int item = 1;
  List_Add(instance, &item);

  EXPECT_EQ(NULL, List_Pop(instance, 1));
}

TEST_F(ListTest, PopThenAdd) {
  int item0 = -42;
  int item1 = 256;
  int item2 = 3141;
  List_Add(instance, &item0);
  List_Add(instance, &item1);
  List_Add(instance, &item2);
  List_Pop(instance, 0);

  List_Add(instance, &item0);

  EXPECT_EQ(&item0, List_Pop(instance, 2));
  EXPECT_EQ(2, List_Count(instance));
}
