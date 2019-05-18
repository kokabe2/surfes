// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "drivers/ssd_driver.h"
#include "spy_runtime_error.h"
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
  SsdDriver instance;

  virtual void SetUp() {
    virtual_ssd = 0xFF;
    instance = SsdDriver_Create(&virtual_ssd, Decode);
    SpyRuntimeError_Reset();
  }

  virtual void TearDown() { SsdDriver_Destroy(&instance); }
};

TEST_F(SsdDriverTest, Create) {
  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ(0x00, virtual_ssd) << "All segments Shall be off";
}

TEST_F(SsdDriverTest, CreateMultipleInstance) {
  uint8_t second_ssd = 0x18;
  SsdDriver second_instance = SsdDriver_Create(&second_ssd, Decode);

  EXPECT_TRUE(second_instance != NULL);
  EXPECT_EQ(0x00, second_ssd) << "All segments Shall be off";
  EXPECT_EQ(0x00, virtual_ssd) << "Shall not be changed";
  SsdDriver_Destroy(&second_instance);
}

TEST_F(SsdDriverTest, CreateWithNullAddress) {
  SsdDriver_Destroy(&instance);

  instance = SsdDriver_Create(NULL, Decode);

  EXPECT_EQ(NULL, instance);
  EXPECT_STREQ("SSD Driver: null I/O address", SpyRuntimeError_GetLastError());
}

TEST_F(SsdDriverTest, CreateWithNullDecoder) {
  SsdDriver_Destroy(&instance);

  virtual_ssd = 0x18;
  instance = SsdDriver_Create(&virtual_ssd, NULL);

  EXPECT_EQ(NULL, instance);
  EXPECT_EQ(0x18, virtual_ssd) << "Shall not be changed";
  EXPECT_STREQ("SSD Driver: null decode function",
               SpyRuntimeError_GetLastError());
}

TEST_F(SsdDriverTest, Destroy) {
  SsdDriver_TurnAllOn(instance);

  SsdDriver_Destroy(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_EQ(0x00, virtual_ssd) << "All segments Shall be off";
}

TEST_F(SsdDriverTest, DestroyWithNull) {
  SsdDriver_TurnAllOn(instance);

  SsdDriver_Destroy(NULL);

  EXPECT_EQ(0xFF, virtual_ssd) << "Shall not be changed";
}

TEST_F(SsdDriverTest, DestroyMoreThanOnce) {
  SsdDriver_Destroy(&instance);

  SsdDriver_Destroy(&instance);

  EXPECT_EQ(NULL, instance) << "Shall not be changed";
  EXPECT_EQ(0x00, virtual_ssd) << "Shall not be changed";
}

TEST_F(SsdDriverTest, TurnAllOn) {
  SsdDriver_TurnAllOn(instance);

  EXPECT_EQ(0xFF, virtual_ssd) << "All segments shall be on";
}

TEST_F(SsdDriverTest, TurnAllOnWithNullInstance) {
  SsdDriver_TurnAllOn(NULL);

  EXPECT_EQ(0x00, virtual_ssd) << "Shall not be changed";
}

TEST_F(SsdDriverTest, TurnAllOff) {
  SsdDriver_TurnAllOn(instance);

  SsdDriver_TurnAllOff(instance);

  EXPECT_EQ(0x00, virtual_ssd) << "All segments Shall be off";
}

TEST_F(SsdDriverTest, TurnAllOffWithNullInstance) {
  SsdDriver_TurnAllOn(instance);

  SsdDriver_TurnAllOff(NULL);

  EXPECT_EQ(0xFF, virtual_ssd) << "Shall not be changed";
}

TEST_F(SsdDriverTest, TurnOn) {
  SsdDriver_TurnOn(instance, 2);

  EXPECT_EQ(0x02, virtual_ssd) << "Specified segment shall be on";
}

TEST_F(SsdDriverTest, TurnOnMultipleSegments) {
  SsdDriver_TurnOn(instance, 4);
  SsdDriver_TurnOn(instance, 5);

  EXPECT_EQ(0x18, virtual_ssd) << "Specified segments shall be on";
}

TEST_F(SsdDriverTest, TurnOnUpperAndLowerBounds) {
  SsdDriver_TurnOn(instance, 1);
  SsdDriver_TurnOn(instance, 8);

  EXPECT_EQ(0x81, virtual_ssd) << "Specified segments shall be on";
}

TEST_F(SsdDriverTest, TurnOnOutOfBounds) {
  SsdDriver_TurnOn(instance, 0);
  SsdDriver_TurnOn(instance, 9);
  SsdDriver_TurnOn(instance, -1);
  SsdDriver_TurnOn(instance, 3141);

  EXPECT_EQ(0x00, virtual_ssd) << "Shall not be changed";
}

TEST_F(SsdDriverTest, TurnOnWithNullInstance) {
  SsdDriver_TurnOn(NULL, 1);

  EXPECT_EQ(0x00, virtual_ssd) << "Shall not be changed";
}

TEST_F(SsdDriverTest, TurnOff) {
  SsdDriver_TurnOn(instance, 2);

  SsdDriver_TurnOff(instance, 2);

  EXPECT_EQ(0x00, virtual_ssd) << "Specified segment shall be off";
}

TEST_F(SsdDriverTest, TurnOffMultipleSegments) {
  SsdDriver_TurnAllOn(instance);

  SsdDriver_TurnOff(instance, 4);
  SsdDriver_TurnOff(instance, 5);

  EXPECT_EQ(0xE7, virtual_ssd) << "Specified segments shall be off";
}

TEST_F(SsdDriverTest, TurnOffUpperAndLowerBounds) {
  SsdDriver_TurnAllOn(instance);

  SsdDriver_TurnOff(instance, 1);
  SsdDriver_TurnOff(instance, 8);

  EXPECT_EQ(0x7E, virtual_ssd) << "Specified segments shall be off";
}

TEST_F(SsdDriverTest, TurnOffOutOfBounds) {
  SsdDriver_TurnAllOn(instance);

  SsdDriver_TurnOff(instance, 0);
  SsdDriver_TurnOff(instance, 9);
  SsdDriver_TurnOff(instance, -1);
  SsdDriver_TurnOff(instance, 3141);

  EXPECT_EQ(0xFF, virtual_ssd) << "Shall not be changed";
}

TEST_F(SsdDriverTest, TurnOffWithNullInstance) {
  SsdDriver_TurnAllOn(instance);

  SsdDriver_TurnOff(NULL, 1);

  EXPECT_EQ(0xFF, virtual_ssd) << "Shall not be changed";
}

TEST_F(SsdDriverTest, IsOn) {
  SsdDriver_TurnOn(instance, 2);

  EXPECT_TRUE(SsdDriver_IsOn(instance, 2));
}

TEST_F(SsdDriverTest, IsOnBeforeTurnOn) {
  EXPECT_FALSE(SsdDriver_IsOn(instance, 4));
}

TEST_F(SsdDriverTest, IsOnOutOfBounds) {
  EXPECT_FALSE(SsdDriver_IsOn(instance, 0));
  EXPECT_FALSE(SsdDriver_IsOn(instance, 9));
  EXPECT_FALSE(SsdDriver_IsOn(instance, -1));
  EXPECT_FALSE(SsdDriver_IsOn(instance, 3141));
}

TEST_F(SsdDriverTest, IsOnWithNullInstance) {
  EXPECT_FALSE(SsdDriver_IsOn(NULL, 5));
}

TEST_F(SsdDriverTest, IsOff) { EXPECT_TRUE(SsdDriver_IsOff(instance, 4)); }
TEST_F(SsdDriverTest, IsOffAfterTurnOn) {
  SsdDriver_TurnOn(instance, 8);

  EXPECT_FALSE(SsdDriver_IsOff(instance, 8));
}

TEST_F(SsdDriverTest, IsOffOutOfBounds) {
  SsdDriver_TurnAllOn(instance);

  EXPECT_TRUE(SsdDriver_IsOff(instance, 0));
  EXPECT_TRUE(SsdDriver_IsOff(instance, 9));
  EXPECT_TRUE(SsdDriver_IsOff(instance, -1));
  EXPECT_TRUE(SsdDriver_IsOff(instance, 3141));
}

TEST_F(SsdDriverTest, IsOffWithNullInstance) {
  SsdDriver_TurnAllOn(instance);

  EXPECT_TRUE(SsdDriver_IsOff(NULL, 1));
}

TEST_F(SsdDriverTest, Set) {
  SsdDriver_TurnAllOn(instance);

  SsdDriver_Set(instance, '0');

  EXPECT_EQ(0x3F, virtual_ssd) << "Only decoded segments shall be on";
}

TEST_F(SsdDriverTest, SetThenTurnOn) {
  SsdDriver_Set(instance, 'F');
  SsdDriver_TurnOn(instance, 2);

  EXPECT_EQ(0x73, virtual_ssd)
      << "Decoded segments and specified segment shall be on";
}

TEST_F(SsdDriverTest, SetWithNullInstance) {
  SsdDriver_Set(instance, '0');

  SsdDriver_Set(NULL, '1');

  EXPECT_EQ(0x3F, virtual_ssd) << "Shall not be changed";
  EXPECT_STREQ("SSD Driver: null instance", SpyRuntimeError_GetLastError());
}

TEST_F(SsdDriverTest, Get) {
  SsdDriver_Set(instance, '0');

  EXPECT_EQ('0', SsdDriver_Get(instance));
}

TEST_F(SsdDriverTest, GetBeforeSet) {
  SsdDriver_TurnOn(instance, 1);

  EXPECT_EQ(0, SsdDriver_Get(NULL));
}

TEST_F(SsdDriverTest, GetWithNullInstance) {
  SsdDriver_Set(instance, '0');

  EXPECT_EQ(0, SsdDriver_Get(NULL));
  EXPECT_STREQ("SSD Driver: null instance", SpyRuntimeError_GetLastError());
}
