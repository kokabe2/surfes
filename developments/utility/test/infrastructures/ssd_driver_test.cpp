// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "ssd_driver.h"
}

namespace {
const uint8_t kSsdDecodings[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,
};

uint8_t Decode(char encoding) {
  if (encoding >= '0' && encoding <= 'F') return kSsdDecodings[encoding - '0'];

  return 0;
}
}  // namespace

class SsdDriverTest : public ::testing::Test {
 protected:
  uint8_t virtual_ssd;
  LedDriver instance;

  virtual void SetUp() {
    virtual_ssd = 0xFF;
    instance = SsdDriver_Create(&virtual_ssd, Decode);
  }

  virtual void TearDown() { LedDriver_Destroy(&instance); }
};

TEST_F(SsdDriverTest, Create) {
  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ(0x00, virtual_ssd) << "All segments Shall be off";
}

TEST_F(SsdDriverTest, CreateMultipleInstance) {
  uint8_t second_ssd = 0x18;
  LedDriver second_instance = SsdDriver_Create(&second_ssd, Decode);

  EXPECT_TRUE(second_instance != NULL);
  EXPECT_EQ(0x00, second_ssd) << "All segments Shall be off";
  EXPECT_EQ(0x00, virtual_ssd) << "Shall not be changed";
  LedDriver_Destroy(&second_instance);
}

TEST_F(SsdDriverTest, CreateWithNullAddress) {
  LedDriver_Destroy(&instance);

  instance = SsdDriver_Create(NULL, Decode);

  EXPECT_EQ(NULL, instance);
}

TEST_F(SsdDriverTest, CreateWithNullDecoder) {
  LedDriver_Destroy(&instance);

  virtual_ssd = 0x18;
  instance = SsdDriver_Create(&virtual_ssd, NULL);

  EXPECT_EQ(NULL, instance);
  EXPECT_EQ(0x18, virtual_ssd) << "Shall not be changed";
}

TEST_F(SsdDriverTest, Destroy) {
  LedDriver_TurnAllOn(instance);

  LedDriver_Destroy(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_EQ(0x00, virtual_ssd) << "All segments Shall be off";
}

TEST_F(SsdDriverTest, TurnAllOn) {
  LedDriver_TurnAllOn(instance);

  EXPECT_EQ(0xFF, virtual_ssd) << "All segments shall be on";
}

TEST_F(SsdDriverTest, TurnAllOff) {
  LedDriver_TurnAllOn(instance);

  LedDriver_TurnAllOff(instance);

  EXPECT_EQ(0x00, virtual_ssd) << "All segments Shall be off";
}

TEST_F(SsdDriverTest, TurnOn) {
  LedDriver_TurnOn(instance, 2);

  EXPECT_EQ(0x02, virtual_ssd) << "Specified segment shall be on";
}

TEST_F(SsdDriverTest, TurnOnMultipleSegments) {
  LedDriver_TurnOn(instance, 4);
  LedDriver_TurnOn(instance, 5);

  EXPECT_EQ(0x18, virtual_ssd) << "Specified segments shall be on";
}

TEST_F(SsdDriverTest, TurnOnUpperAndLowerBounds) {
  LedDriver_TurnOn(instance, 1);
  LedDriver_TurnOn(instance, 8);

  EXPECT_EQ(0x81, virtual_ssd) << "Specified segments shall be on";
}

TEST_F(SsdDriverTest, TurnOff) {
  LedDriver_TurnOn(instance, 2);

  LedDriver_TurnOff(instance, 2);

  EXPECT_EQ(0x00, virtual_ssd) << "Specified segment shall be off";
}

TEST_F(SsdDriverTest, TurnOffMultipleSegments) {
  LedDriver_TurnAllOn(instance);

  LedDriver_TurnOff(instance, 4);
  LedDriver_TurnOff(instance, 5);

  EXPECT_EQ(0xE7, virtual_ssd) << "Specified segments shall be off";
}

TEST_F(SsdDriverTest, TurnOffUpperAndLowerBounds) {
  LedDriver_TurnAllOn(instance);

  LedDriver_TurnOff(instance, 1);
  LedDriver_TurnOff(instance, 8);

  EXPECT_EQ(0x7E, virtual_ssd) << "Specified segments shall be off";
}

TEST_F(SsdDriverTest, IsOn) {
  LedDriver_TurnOn(instance, 2);

  EXPECT_TRUE(LedDriver_IsOn(instance, 2));
}

TEST_F(SsdDriverTest, IsOff) { EXPECT_TRUE(LedDriver_IsOff(instance, 4)); }

TEST_F(SsdDriverTest, Set) {
  LedDriver_TurnAllOn(instance);

  SsdDriver_Set(instance, '0');

  EXPECT_EQ(0x3F, virtual_ssd) << "Only decoded segments shall be on";
}

TEST_F(SsdDriverTest, SetThenTurnOn) {
  SsdDriver_Set(instance, 'F');
  LedDriver_TurnOn(instance, 2);

  EXPECT_EQ(0x73, virtual_ssd)
      << "Decoded segments and specified segment shall be on";
}

TEST_F(SsdDriverTest, SetWithNullInstance) {
  SsdDriver_Set(instance, '0');

  SsdDriver_Set(NULL, '1');

  EXPECT_EQ(0x3F, virtual_ssd) << "Shall not be changed";
}

TEST_F(SsdDriverTest, Get) {
  SsdDriver_Set(instance, '0');

  EXPECT_EQ('0', SsdDriver_Get(instance));
}

TEST_F(SsdDriverTest, GetBeforeSet) {
  LedDriver_TurnOn(instance, 1);

  EXPECT_EQ(0, SsdDriver_Get(NULL));
}

TEST_F(SsdDriverTest, GetWithNullInstance) {
  SsdDriver_Set(instance, '0');

  EXPECT_EQ(0, SsdDriver_Get(NULL));
}
