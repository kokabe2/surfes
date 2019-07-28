// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "impl/fake_alarmhandler.h"
#include "one_shot_timer.h"
}

namespace {
const int kDummyParameter = 1234;
void* given_parameter;
bool was_ran;
void FakeFunction(void* parameter) {
  was_ran = true;
  given_parameter = parameter;
}
}  // namespace

class OneShotTimerTest : public ::testing::Test {
 protected:
  Timer instance;

  virtual void SetUp() {
    fake_alarmhandler_init();
    instance = OneShotTimer_Create(FakeFunction, 10, (void*)&kDummyParameter);
    was_ran = false;
    given_parameter = NULL;
  }
  virtual void TearDown() { Timer_Destroy(&instance); }
};

TEST_F(OneShotTimerTest, Create) {
  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ(TA_HLNG, fake_alarmhandler_getAttribute(0));
  EXPECT_TRUE(fake_alarmhandler_isActive(0));
  EXPECT_EQ(10, fake_alarmhandler_getLeftTime(0));
}

TEST_F(OneShotTimerTest, CreateWithNullFunction) {
  EXPECT_EQ(NULL, OneShotTimer_Create(NULL, 10, (void*)&kDummyParameter));
}

TEST_F(OneShotTimerTest, CreateWithTimeLessThanZero) {
  EXPECT_EQ(NULL,
            OneShotTimer_Create(FakeFunction, -1, (void*)&kDummyParameter));
}

TEST_F(OneShotTimerTest, RunScheduledFunction) {
  EXPECT_FALSE(was_ran);

  fake_alarmhandler_countdown(0, 20);

  EXPECT_TRUE(was_ran);
  EXPECT_EQ((void*)&kDummyParameter, given_parameter);
  EXPECT_FALSE(fake_alarmhandler_isActive(0));
  EXPECT_EQ(0, fake_alarmhandler_getLeftTime(0));
}

TEST_F(OneShotTimerTest, Destroy) {
  EXPECT_TRUE(fake_alarmhandler_isCreated(0));
  Timer_Destroy(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_FALSE(fake_alarmhandler_isCreated(0));
}

TEST_F(OneShotTimerTest, DestroyAfterDestroy) {
  Timer_Destroy(&instance);
  Timer_Destroy(&instance);

  SUCCEED();
}

TEST_F(OneShotTimerTest, DestroyWithNull) {
  Timer_Destroy(NULL);

  SUCCEED();
}

TEST_F(OneShotTimerTest, Pause) {
  Timer_Pause(instance);

  EXPECT_FALSE(fake_alarmhandler_isActive(0));
  EXPECT_EQ(0, fake_alarmhandler_getLeftTime(0));
}

TEST_F(OneShotTimerTest, PauseWithNull) {
  Timer_Pause(NULL);

  SUCCEED();
}

TEST_F(OneShotTimerTest, Resume) {
  fake_alarmhandler_countdown(0, 5);
  Timer_Pause(instance);

  Timer_Resume(instance);

  EXPECT_FALSE(was_ran);
  EXPECT_TRUE(fake_alarmhandler_isActive(0));
  EXPECT_EQ(5, fake_alarmhandler_getLeftTime(0));
}

TEST_F(OneShotTimerTest, ResumeWhenNotPauseed) {
  fake_alarmhandler_countdown(0, 5);

  Timer_Resume(instance);

  EXPECT_FALSE(was_ran);
  EXPECT_TRUE(fake_alarmhandler_isActive(0));
  EXPECT_EQ(5, fake_alarmhandler_getLeftTime(0));
}

TEST_F(OneShotTimerTest, ResumeAfterStopped) {
  fake_alarmhandler_countdown(0, 20);
  was_ran = false;

  Timer_Resume(instance);

  EXPECT_FALSE(was_ran);
  EXPECT_FALSE(fake_alarmhandler_isActive(0));
  EXPECT_EQ(0, fake_alarmhandler_getLeftTime(0));
}

TEST_F(OneShotTimerTest, ResumeWithNull) {
  Timer_Resume(NULL);

  SUCCEED();
}
