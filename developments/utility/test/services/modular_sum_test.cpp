// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "modular_sum.h"
}

class ModularSumTest : public ::testing::Test {
 protected:
  uint32_t dummy_data[16]{
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
  };
};

TEST_F(ModularSumTest, Verify) {
  uint32_t sum = ModularSum_Verify(dummy_data, sizeof(dummy_data));

  EXPECT_EQ(120, sum);
}

TEST_F(ModularSumTest, VerifyWithSizeLessThanOne) {
  uint32_t sum = ModularSum_Verify(dummy_data, 0);

  EXPECT_NE(0, sum);
}

TEST_F(ModularSumTest, VerifyWithSizeNotMultiplesOfFour) {
  uint32_t sum = ModularSum_Verify(dummy_data, sizeof(dummy_data) - 1);

  EXPECT_NE(0, sum);
}

TEST_F(ModularSumTest, Calculate) {
  uint32_t checksum = ModularSum_Calculate(dummy_data, sizeof(dummy_data));

  EXPECT_EQ(0xFFFFFF88, checksum);
}

TEST_F(ModularSumTest, CalculateWithSizeLessThanOne) {
  uint32_t checksum = ModularSum_Calculate(dummy_data, 0);

  EXPECT_EQ(0, checksum);
}

TEST_F(ModularSumTest, CalculateWithSizeNotMultiplesOfFour) {
  uint32_t checksum = ModularSum_Calculate(dummy_data, sizeof(dummy_data) - 1);

  EXPECT_EQ(0, checksum);
}
