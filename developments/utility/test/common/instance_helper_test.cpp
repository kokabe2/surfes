// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "instance_helper.h"
}

typedef struct {
  char padding[16];
} InstanceHelperDummyStruct, *InstanceHelperDummy;

class InstanceHelperTest : public ::testing::Test {
 protected:
  InstanceHelperDummy instance;

  virtual void SetUp() {
    instance = (InstanceHelperDummy)InstanceHelper_New(
        sizeof(InstanceHelperDummyStruct));
  }

  virtual void TearDown() { InstanceHelper_Delete(&instance); }
};

TEST_F(InstanceHelperTest, Create) {
  InstanceHelperDummyStruct expected = {0};

  ASSERT_TRUE(instance != NULL);
  EXPECT_EQ(0, memcmp(instance, &expected, sizeof(InstanceHelperDummyStruct)));
}

TEST_F(InstanceHelperTest, CreateWithSizeLessThanOne) {
  InstanceHelper_Delete(&instance);

  instance = (InstanceHelperDummy)InstanceHelper_New(0);

  EXPECT_EQ(NULL, instance);
}

TEST_F(InstanceHelperTest, Delete) {
  InstanceHelper_Delete(&instance);

  EXPECT_EQ(NULL, instance);
}

TEST_F(InstanceHelperTest, DeleteWithNull) {
  InstanceHelper_Delete(NULL);

  SUCCEED();
}

TEST_F(InstanceHelperTest, DeleteMoreThanOnce) {
  InstanceHelper_Delete(&instance);
  InstanceHelper_Delete(&instance);

  EXPECT_EQ(NULL, instance);
}
