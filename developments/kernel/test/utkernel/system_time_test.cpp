// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "impl/fake_time.h"
#include "system_time.h"
}

class SystemTimeTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    fake_time_init();
    SystemTime_Create(0x0123456789ABCDEF);
  }
  virtual void TearDown() { SystemTime_Destroy(); }
};

TEST_F(SystemTimeTest, Get) {
  EXPECT_EQ(0x0123456789ABCDEF, SystemTime_get());
  fake_time_increment(1);
  EXPECT_EQ(0x0123456789ABCDF0, SystemTime_get());
  fake_time_increment(0x1234567890000000);
  EXPECT_EQ(0x13579BE019ABCDF0, SystemTime_get());
  fake_time_increment(0x9000000000000001);
  EXPECT_EQ(0xA3579BE019ABCDF1, SystemTime_get());
}
