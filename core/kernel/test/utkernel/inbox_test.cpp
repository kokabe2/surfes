// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "impl/fake_mailbox.h"
#include "impl/fake_memorypool.h"
#include "inbox.h"
}

class InboxTest : public ::testing::Test {
 protected:
  Inbox instance;

  virtual void SetUp() {
    fake_mailbox_init();
    fake_memorypool_init();
    instance = Inbox_Create(1024);
  }
  virtual void TearDown() { Inbox_Destroy(&instance); }
};

TEST_F(InboxTest, Create) {
  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ((TA_TFIFO | TA_MFIFO), fake_mailbox_getAttribute(0));
  EXPECT_EQ((TA_TFIFO | TA_RNG0), fake_memorypool_getAttribute(0));
  EXPECT_EQ(1024, fake_memorypool_getSize(0));
}

TEST_F(InboxTest, CreateWithOutOfRangeInboxSize) {
  EXPECT_EQ(NULL, Inbox_Create(0));
  EXPECT_EQ(NULL, Inbox_Create(-128));
  EXPECT_EQ(NULL, Inbox_Create(kIsMaxSize + 1));
}

TEST_F(InboxTest, Destroy) {
  EXPECT_TRUE(fake_mailbox_isCreated(0));
  EXPECT_TRUE(fake_memorypool_isCreated(0));

  Inbox_Destroy(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_FALSE(fake_mailbox_isCreated(0));
  EXPECT_FALSE(fake_memorypool_isCreated(0));
}

TEST_F(InboxTest, DestroyAfterDestroy) {
  Inbox_Destroy(&instance);
  Inbox_Destroy(&instance);

  SUCCEED();
}

TEST_F(InboxTest, DestroyWithNull) {
  Inbox_Destroy(NULL);

  SUCCEED();
}

TEST_F(InboxTest, Post) {
  int dummy_message[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  EXPECT_TRUE(
      Inbox_Post(instance, sizeof(dummy_message), (void*)dummy_message));
  EXPECT_EQ(
      0, memcmp(dummy_message,
                (void*)((uintptr_t)fake_mailbox_getLastMail(0) + sizeof(T_MSG)),
                sizeof(dummy_message)));
  EXPECT_EQ(sizeof(T_MSG) + sizeof(dummy_message),
            fake_memorypool_getUsedSize(0));
}

TEST_F(InboxTest, PostWithMessageSizeMoreThanFreeSize) {
  char dummy_message[512] = {0};

  EXPECT_TRUE(
      Inbox_Post(instance, sizeof(dummy_message), (void*)dummy_message));
  EXPECT_FALSE(
      Inbox_Post(instance, sizeof(dummy_message), (void*)dummy_message));
  EXPECT_EQ(sizeof(T_MSG) + sizeof(dummy_message),
            fake_memorypool_getUsedSize(0));
}

TEST_F(InboxTest, PostWithNullInstance) {
  int dummy_message[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  EXPECT_FALSE(Inbox_Post(NULL, sizeof(dummy_message), (void*)dummy_message));
}

TEST_F(InboxTest, PostWithOutOfRangeSize) {
  int dummy_message[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  EXPECT_FALSE(Inbox_Post(instance, 0, (void*)dummy_message));
  EXPECT_FALSE(Inbox_Post(instance, -16, (void*)dummy_message));
}

TEST_F(InboxTest, PostWithNullMessage) {
  EXPECT_FALSE(Inbox_Post(instance, 16, NULL));
  EXPECT_EQ(0, fake_memorypool_getUsedSize(0));
}

TEST_F(InboxTest, Get) {
  int dummy_message[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  Inbox_Post(instance, sizeof(dummy_message), (void*)dummy_message);

  void* message = Inbox_Get(instance);

  EXPECT_EQ(0, memcmp(dummy_message, message, sizeof(dummy_message)));
}

TEST_F(InboxTest, GetAfterGet) {
  int dummy_message[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  Inbox_Post(instance, sizeof(dummy_message), (void*)dummy_message);
  void* message = Inbox_Get(instance);

  EXPECT_EQ(sizeof(T_MSG) + sizeof(dummy_message),
            fake_memorypool_getUsedSize(0));
  EXPECT_EQ(NULL, Inbox_Get(instance));
  EXPECT_EQ(0, fake_memorypool_getUsedSize(0));
}

TEST_F(InboxTest, GetBeforePost) { EXPECT_EQ(NULL, Inbox_Get(instance)); }

TEST_F(InboxTest, GetWithNull) { EXPECT_EQ(NULL, Inbox_Get(NULL)); }
