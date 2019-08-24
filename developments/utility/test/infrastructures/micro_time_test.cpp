// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "micro_time.h"
}

class MicroTimeTest : public ::testing::Test {
 protected:
  virtual void SetUp() { MicroTime_Create(); }
  virtual void TearDown() {}
};

TEST_F(MicroTimeTest, TimeIsZeroAfterCreate) { EXPECT_EQ(0, MicroTime_Get()); }

TEST_F(MicroTimeTest, MicroTimeIncrements) {
  MicroTime_Increase(5);
  EXPECT_EQ(5, MicroTime_Get());
  MicroTime_Increase(5);
  EXPECT_EQ(10, MicroTime_Get());
}

TEST_F(MicroTimeTest, NoChangesIfNoIncrement) {
  MicroTime_Increase(100);
  EXPECT_EQ(100, MicroTime_Get());
  EXPECT_EQ(100, MicroTime_Get());
  EXPECT_EQ(100, MicroTime_Get());
}
