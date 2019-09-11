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
  int items[8];
  List instance;
  virtual void SetUp() {
    destruction_count = 0;
    last_item = NULL;
    memset(items, 0, sizeof(items));
    instance = List_Create(CompareInteger, DummyDelete);
  }
  virtual void TearDown() { List_Destroy(&instance); }

  void AssertInitialCondition(List l) {
    ASSERT_TRUE(l != NULL);
    EXPECT_EQ(0, List_Count(l));
    EXPECT_EQ(NULL, List_Get(l, 0));
    EXPECT_EQ(NULL, List_getFirst(l));
    EXPECT_EQ(NULL, List_getLast(l));
  }
  void AssertDeletion(int count) {
    EXPECT_EQ(count, destruction_count);
    EXPECT_EQ(&items[count - 1], last_item);
  }
  void AssertAddition(int count) {
    EXPECT_EQ(count, List_Count(instance));
    EXPECT_EQ(&items[0], List_getFirst(instance));
    EXPECT_EQ(&items[count - 1], List_getLast(instance));
    for (int i = 0; i < count; ++i) EXPECT_EQ(&items[i], List_Get(instance, i));
  }
  void AddItems(int count) {
    for (int i = 0; i < count; ++i) List_Add(instance, &items[i]);
  }
};

TEST_F(ListTest, Create) { AssertInitialCondition(instance); }

TEST_F(ListTest, CreateWithNullComparator) {
  List l = List_Create(NULL, DummyDelete);

  AssertInitialCondition(l);

  List_Destroy(&l);
}

TEST_F(ListTest, CreateWithNullDestructor) {
  List l = List_Create(CompareInteger, NULL);

  AssertInitialCondition(l);

  List_Destroy(&l);
}

TEST_F(ListTest, Destroy) {
  AddItems(4);

  List_Destroy(&instance);

  EXPECT_EQ(NULL, instance);
  AssertDeletion(4);
}

TEST_F(ListTest, DestroyAfterDestroy) {
  List_Destroy(&instance);
  List_Destroy(&instance);

  SUCCEED();
}

TEST_F(ListTest, DestroyWithNull) {
  List_Destroy(NULL);

  SUCCEED();
}

TEST_F(ListTest, AddOneItem) {
  AddItems(1);

  AssertAddition(1);
}

TEST_F(ListTest, Add) {
  AddItems(3);

  AssertAddition(3);
}

TEST_F(ListTest, AddWithNullInstance) {
  int item;

  List_Add(NULL, &item);

  SUCCEED();
}

TEST_F(ListTest, CountWithNull) { EXPECT_EQ(0, List_Count(NULL)); }

TEST_F(ListTest, GetWithNull) { EXPECT_EQ(NULL, List_Get(NULL, 0)); }

TEST_F(ListTest, GetWithIndexMoreThanAdded) {
  AddItems(1);

  EXPECT_EQ(NULL, List_Get(NULL, 1));
  EXPECT_EQ(NULL, List_Get(NULL, 124));
}

TEST_F(ListTest, GetWithIndexLessThanZero) {
  EXPECT_EQ(NULL, List_Get(instance, -1));
  EXPECT_EQ(NULL, List_Get(instance, -243));
}

TEST_F(ListTest, GetFirstWithNull) { EXPECT_EQ(NULL, List_getFirst(NULL)); }

TEST_F(ListTest, GetLastWithNull) { EXPECT_EQ(NULL, List_getLast(NULL)); }

TEST_F(ListTest, Clear) {
  AddItems(8);

  List_Clear(instance);

  AssertInitialCondition(instance);
  AssertDeletion(8);
}

TEST_F(ListTest, ClearThenAdd) {
  AddItems(5);
  List_Clear(instance);
  AddItems(6);

  AssertAddition(6);
}

TEST_F(ListTest, ClearWithNull) {
  List_Clear(NULL);

  SUCCEED();
}

TEST_F(ListTest, Find) {
  items[2] = 1234;
  int match = items[2];
  AddItems(4);

  EXPECT_EQ(&items[2], (int*)List_Find(instance, &match));
}

TEST_F(ListTest, FindWithNotAddedItem) {
  int match = -128;
  AddItems(4);

  EXPECT_EQ(NULL, (int*)List_Find(instance, &match));
}

TEST_F(ListTest, FindWithNullInstance) {
  EXPECT_EQ(NULL, List_Find(NULL, items));
}

TEST_F(ListTest, FindWithNullItem) {
  EXPECT_EQ(NULL, List_Find(instance, NULL));
}

TEST_F(ListTest, FindIfComparatorIsNotSet) {
  List l = List_Create(NULL, DummyDelete);
  AddItems(2);

  EXPECT_EQ(NULL, List_Find(l, &items[1]));

  List_Destroy(&l);
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
