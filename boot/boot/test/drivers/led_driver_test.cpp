// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "drivers/led_driver.h"
#include "utilities/spy_runtime_error.h"
}

namespace {
const uint8_t kLedBits[8] = {0x01, 0x02, 0x00, 0x08, 0x10, 0x00, 0x40, 0x80};
uint8_t Decode(int led_number) { return kLedBits[led_number - 1]; }
}  // namespace

class LedDriverTest : public ::testing::Test {
 protected:
  uint8_t virtual_leds;
  LedDriver instance;

  virtual void SetUp() {
    virtual_leds = 0xF0;
    instance = LedDriver_Create(&virtual_leds, Decode);
    SpyRuntimeError_Reset();
  }

  virtual void TearDown() { LedDriver_Destroy(&instance); }
};

TEST_F(LedDriverTest, Create) {
  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ(0x20, virtual_leds) << "All LEDs Shall be off";
}

TEST_F(LedDriverTest, CreateMultipleInstance) {
  uint8_t second_leds = 0x18;
  LedDriver second_instance = LedDriver_Create(&second_leds, Decode);

  EXPECT_TRUE(second_instance != NULL);
  EXPECT_EQ(0x00, second_leds) << "All LEDs Shall be off";
  EXPECT_EQ(0x20, virtual_leds) << "Shall not be changed";
  LedDriver_Destroy(&second_instance);
}

TEST_F(LedDriverTest, CreateWithNullAddress) {
  LedDriver_Destroy(&instance);

  instance = LedDriver_Create(NULL, Decode);

  EXPECT_EQ(NULL, instance);
  EXPECT_STREQ("LED Driver: null I/O address", SpyRuntimeError_GetLastError());
}

TEST_F(LedDriverTest, CreateWithNullDecoder) {
  LedDriver_Destroy(&instance);

  virtual_leds = 0xFB;
  instance = LedDriver_Create(&virtual_leds, NULL);

  EXPECT_EQ(NULL, instance);
  EXPECT_EQ(0xFB, virtual_leds) << "Shall not be changed";
  EXPECT_STREQ("LED Driver: null decode function",
               SpyRuntimeError_GetLastError());
}

TEST_F(LedDriverTest, Destroy) {
  LedDriver_TurnAllOn(instance);

  LedDriver_Destroy(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_EQ(0x20, virtual_leds) << "All LEDs Shall be off";
}

TEST_F(LedDriverTest, DestroyWithNull) {
  LedDriver_TurnAllOn(instance);

  LedDriver_Destroy(NULL);

  EXPECT_EQ(0xFB, virtual_leds) << "Shall not be changed";
}

TEST_F(LedDriverTest, DestroyMoreThanOnce) {
  LedDriver_Destroy(&instance);

  LedDriver_Destroy(&instance);

  EXPECT_EQ(NULL, instance) << "Shall not be changed";
  EXPECT_EQ(0x20, virtual_leds) << "Shall not be changed";
}

TEST_F(LedDriverTest, TurnAllOn) {
  LedDriver_TurnAllOn(instance);

  EXPECT_EQ(0xFB, virtual_leds) << "All LEDs shall be on";
}

TEST_F(LedDriverTest, TurnAllOnWithNullInstance) {
  LedDriver_TurnAllOn(NULL);

  EXPECT_EQ(0x20, virtual_leds) << "Shall not be changed";
  EXPECT_STREQ("LED Driver: null instance", SpyRuntimeError_GetLastError());
}

TEST_F(LedDriverTest, TurnAllOff) {
  LedDriver_TurnAllOn(instance);

  LedDriver_TurnAllOff(instance);

  EXPECT_EQ(0x20, virtual_leds) << "All LEDs Shall be off";
}

TEST_F(LedDriverTest, TurnAllOffWithNullInstance) {
  LedDriver_TurnAllOn(instance);

  LedDriver_TurnAllOff(NULL);

  EXPECT_EQ(0xFB, virtual_leds) << "Shall not be changed";
  EXPECT_STREQ("LED Driver: null instance", SpyRuntimeError_GetLastError());
}

TEST_F(LedDriverTest, TurnOn) {
  LedDriver_TurnOn(instance, 2);

  EXPECT_EQ(0x22, virtual_leds) << "Specified LED shall be on";
}

TEST_F(LedDriverTest, TurnOnMultipleLeds) {
  LedDriver_TurnOn(instance, 4);
  LedDriver_TurnOn(instance, 5);

  EXPECT_EQ(0x38, virtual_leds) << "Specified LEDs shall be on";
}

TEST_F(LedDriverTest, TurnOnUpperAndLowerBounds) {
  LedDriver_TurnOn(instance, 1);
  LedDriver_TurnOn(instance, 8);

  EXPECT_EQ(0xA1, virtual_leds) << "Specified LEDs shall be on";
}

TEST_F(LedDriverTest, TurnOnOutOfBounds) {
  LedDriver_TurnOn(instance, 0);
  LedDriver_TurnOn(instance, 9);
  LedDriver_TurnOn(instance, -1);
  LedDriver_TurnOn(instance, 3141);

  EXPECT_EQ(0x20, virtual_leds) << "Shall not be changed";
  EXPECT_STREQ("LED Driver: out-of-bounds LED", SpyRuntimeError_GetLastError());
  EXPECT_EQ(3141, SpyRuntimeError_GetLastParameter());
}

TEST_F(LedDriverTest, TurnOnWithInvalidLedNumber) {
  LedDriver_TurnOn(instance, 3);

  EXPECT_EQ(0x20, virtual_leds) << "Shall not be changed";
}

TEST_F(LedDriverTest, TurnOnWithNullInstance) {
  LedDriver_TurnOn(NULL, 1);

  EXPECT_EQ(0x20, virtual_leds) << "Shall not be changed";
  EXPECT_STREQ("LED Driver: null instance", SpyRuntimeError_GetLastError());
}

TEST_F(LedDriverTest, TurnOff) {
  LedDriver_TurnOn(instance, 2);

  LedDriver_TurnOff(instance, 2);

  EXPECT_EQ(0x20, virtual_leds) << "Specified LED shall be off";
}

TEST_F(LedDriverTest, TurnOffMultipleLeds) {
  LedDriver_TurnAllOn(instance);

  LedDriver_TurnOff(instance, 4);
  LedDriver_TurnOff(instance, 5);

  EXPECT_EQ(0xE3, virtual_leds) << "Specified LEDs shall be off";
}

TEST_F(LedDriverTest, TurnOffUpperAndLowerBounds) {
  LedDriver_TurnAllOn(instance);

  LedDriver_TurnOff(instance, 1);
  LedDriver_TurnOff(instance, 8);

  EXPECT_EQ(0x7A, virtual_leds) << "Specified LEDs shall be off";
}

TEST_F(LedDriverTest, TurnOffOutOfBounds) {
  LedDriver_TurnAllOn(instance);

  LedDriver_TurnOff(instance, 0);
  LedDriver_TurnOff(instance, 9);
  LedDriver_TurnOff(instance, -1);
  LedDriver_TurnOff(instance, 3141);

  EXPECT_EQ(0xFB, virtual_leds) << "Shall not be changed";
  EXPECT_STREQ("LED Driver: out-of-bounds LED", SpyRuntimeError_GetLastError());
  EXPECT_EQ(3141, SpyRuntimeError_GetLastParameter());
}

TEST_F(LedDriverTest, TurnOffWithInvalidLedNumber) {
  LedDriver_TurnAllOn(instance);

  LedDriver_TurnOff(instance, 6);

  EXPECT_EQ(0xFB, virtual_leds) << "Shall not be changed";
}

TEST_F(LedDriverTest, TurnOffWithNullInstance) {
  LedDriver_TurnAllOn(instance);

  LedDriver_TurnOff(NULL, 1);

  EXPECT_EQ(0xFB, virtual_leds) << "Shall not be changed";
  EXPECT_STREQ("LED Driver: null instance", SpyRuntimeError_GetLastError());
}

TEST_F(LedDriverTest, IsOn) {
  LedDriver_TurnOn(instance, 2);

  EXPECT_TRUE(LedDriver_IsOn(instance, 2));
}

TEST_F(LedDriverTest, IsOnBeforeTurnOn) {
  EXPECT_FALSE(LedDriver_IsOn(instance, 4));
}

TEST_F(LedDriverTest, IsOnOutOfBounds) {
  EXPECT_FALSE(LedDriver_IsOn(instance, 0));
  EXPECT_FALSE(LedDriver_IsOn(instance, 9));
  EXPECT_FALSE(LedDriver_IsOn(instance, -1));
  EXPECT_FALSE(LedDriver_IsOn(instance, 3141));
  EXPECT_STREQ("LED Driver: out-of-bounds LED", SpyRuntimeError_GetLastError());
  EXPECT_EQ(3141, SpyRuntimeError_GetLastParameter());
}

TEST_F(LedDriverTest, IsOnWithInvalidLedNumber) {
  EXPECT_FALSE(LedDriver_IsOn(instance, 3));
}

TEST_F(LedDriverTest, IsOnWithNullInstance) {
  EXPECT_FALSE(LedDriver_IsOn(NULL, 5));
  EXPECT_STREQ("LED Driver: null instance", SpyRuntimeError_GetLastError());
}

TEST_F(LedDriverTest, IsOff) { EXPECT_TRUE(LedDriver_IsOff(instance, 4)); }
TEST_F(LedDriverTest, IsOffAfterTurnOn) {
  LedDriver_TurnOn(instance, 8);

  EXPECT_FALSE(LedDriver_IsOff(instance, 8));
}

TEST_F(LedDriverTest, IsOffOutOfBounds) {
  LedDriver_TurnAllOn(instance);

  EXPECT_TRUE(LedDriver_IsOff(instance, 0));
  EXPECT_TRUE(LedDriver_IsOff(instance, 9));
  EXPECT_TRUE(LedDriver_IsOff(instance, -1));
  EXPECT_TRUE(LedDriver_IsOff(instance, 3141));
  EXPECT_STREQ("LED Driver: out-of-bounds LED", SpyRuntimeError_GetLastError());
  EXPECT_EQ(3141, SpyRuntimeError_GetLastParameter());
}

TEST_F(LedDriverTest, IsOffWithInvalidLedNumber) {
  LedDriver_TurnAllOn(instance);

  EXPECT_TRUE(LedDriver_IsOff(instance, 6));
}

TEST_F(LedDriverTest, IsOffWithNullInstance) {
  LedDriver_TurnAllOn(instance);

  EXPECT_TRUE(LedDriver_IsOff(NULL, 1));
  EXPECT_STREQ("LED Driver: null instance", SpyRuntimeError_GetLastError());
}
