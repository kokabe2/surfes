
// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "heap.h"
#include "impl/fake_memorypool.h"
}

namespace {
char heap_area[512];
}  //

class HeapTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    memset(heap_area, 0xFF, 512);
    fake_memorypool_init();
    Heap_Create(512, heap_area);
  }
  virtual void TearDown() { Heap_Destroy(); }
};

TEST_F(HeapTest, Create) {
  EXPECT_TRUE(fake_memorypool_isCreated(0));
  EXPECT_EQ((TA_TFIFO | TA_RNG0 | TA_USERBUF), fake_memorypool_getAttribute(0));
  EXPECT_EQ(512, fake_memorypool_getSize(0));
}

TEST_F(HeapTest, CreateWithSizeZeroOrLess) {
  Heap_Create(0, heap_area);
  Heap_Create(-1234, heap_area);

  EXPECT_FALSE(fake_memorypool_isCreated(1));
  EXPECT_FALSE(fake_memorypool_isCreated(2));
}

TEST_F(HeapTest, CreateWithNull) {
  Heap_Create(512, NULL);

  EXPECT_FALSE(fake_memorypool_isCreated(1));
}

TEST_F(HeapTest, CreateMutilpleTimes) {
  Heap_Create(512, heap_area);
  Heap_Create(512, heap_area);

  EXPECT_FALSE(fake_memorypool_isCreated(1));
  EXPECT_FALSE(fake_memorypool_isCreated(2));
}

TEST_F(HeapTest, Destroy) {
  Heap_Destroy();

  EXPECT_FALSE(fake_memorypool_isCreated(0));
}

TEST_F(HeapTest, DestroyMultipleTimes) {
  Heap_Destroy();
  Heap_Destroy();

  SUCCEED();
}

TEST_F(HeapTest, Allocate) {
  char expected[16] = {0};

  void *memory = Heap_Allocate(16);

  EXPECT_EQ(heap_area + sizeof(W), memory);
  EXPECT_EQ(16 + sizeof(W), fake_memorypool_getUsedSize(0));
  EXPECT_EQ(0, memcmp(memory, expected, 16));
}

TEST_F(HeapTest, AllocateWithSizeZeroOrLess) {
  EXPECT_EQ(NULL, Heap_Allocate(0));
  EXPECT_EQ(NULL, Heap_Allocate(-11));
}

TEST_F(HeapTest, Release) {
  void *memory = Heap_Allocate(16);

  Heap_Release(memory);

  EXPECT_EQ(0, fake_memorypool_getUsedSize(0));
}

TEST_F(HeapTest, ReleaseWithNull) {
  Heap_Release(NULL);

  SUCCEED();
}
