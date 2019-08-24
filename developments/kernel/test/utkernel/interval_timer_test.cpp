// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "impl/fake_cyclichandler.h"
#include "interval_timer.h"
}

namespace {
const int kDummyParameter = 1234;
void* given_parameter;
int ran_count;
void FakeFunction(void* parameter) {
  ran_count++;
  given_parameter = parameter;
}
}  // namespace

class IntervalTimerTest : public ::testing::Test {
 protected:
  Timer instance;

  virtual void SetUp() {
    fake_cyclichandler_init();
    instance = IntervalTimer_Create(FakeFunction, 10, (void*)&kDummyParameter);
    ran_count = 0;
    given_parameter = NULL;
  }
  virtual void TearDown() { Timer_Destroy(&instance); }
};

TEST_F(IntervalTimerTest, Create) {
  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ((TA_HLNG | TA_STA | TA_PHS), fake_cyclichandler_getAttribute(0));
  EXPECT_TRUE(fake_cyclichandler_isActive(0));
  EXPECT_EQ(10, fake_cyclichandler_getLeftTime(0));
  EXPECT_EQ(10, fake_cyclichandler_getCyclicTime(0));
}

TEST_F(IntervalTimerTest, CreateWithNullFunction) {
  EXPECT_EQ(NULL, IntervalTimer_Create(NULL, 10, (void*)&kDummyParameter));
}

TEST_F(IntervalTimerTest, CreateWithTimeLessThanZero) {
  EXPECT_EQ(NULL,
            IntervalTimer_Create(FakeFunction, -1, (void*)&kDummyParameter));
}

TEST_F(IntervalTimerTest, RunScheduledFunction) {
  EXPECT_EQ(0, ran_count);

  fake_cyclichandler_countdown(0, 10);

  EXPECT_EQ(1, ran_count);
  EXPECT_EQ((void*)&kDummyParameter, given_parameter);
  EXPECT_TRUE(fake_cyclichandler_isActive(0));
  EXPECT_EQ(10, fake_cyclichandler_getLeftTime(0));
}

TEST_F(IntervalTimerTest, RunMultipleTimes) {
  fake_cyclichandler_countdown(0, 123);

  EXPECT_EQ(12, ran_count);
  EXPECT_EQ(7, fake_cyclichandler_getLeftTime(0));
}

TEST_F(IntervalTimerTest, Destroy) {
  EXPECT_TRUE(fake_cyclichandler_isCreated(0));
  Timer_Destroy(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_FALSE(fake_cyclichandler_isCreated(0));
}

TEST_F(IntervalTimerTest, DestroyAfterDestroy) {
  Timer_Destroy(&instance);
  Timer_Destroy(&instance);

  SUCCEED();
}

TEST_F(IntervalTimerTest, DestroyWithNull) {
  Timer_Destroy(NULL);

  SUCCEED();
}

TEST_F(IntervalTimerTest, Pause) {
  Timer_Pause(instance);
  fake_cyclichandler_countdown(0, 10);

  EXPECT_EQ(0, ran_count);
  EXPECT_FALSE(fake_cyclichandler_isActive(0));
}

TEST_F(IntervalTimerTest, PauseWithNull) {
  Timer_Pause(NULL);

  SUCCEED();
}

TEST_F(IntervalTimerTest, Resume) {
  fake_cyclichandler_countdown(0, 5);
  Timer_Pause(instance);

  Timer_Resume(instance);

  EXPECT_EQ(0, ran_count);
  EXPECT_TRUE(fake_cyclichandler_isActive(0));
  EXPECT_EQ(5, fake_cyclichandler_getLeftTime(0));
}

TEST_F(IntervalTimerTest, ResumeWhenNotPauseed) {
  fake_cyclichandler_countdown(0, 5);

  Timer_Resume(instance);

  EXPECT_EQ(0, ran_count);
  EXPECT_TRUE(fake_cyclichandler_isActive(0));
  EXPECT_EQ(5, fake_cyclichandler_getLeftTime(0));
}

TEST_F(IntervalTimerTest, ResumeWithNull) {
  Timer_Pause(instance);

  Timer_Resume(NULL);

  SUCCEED();
}
