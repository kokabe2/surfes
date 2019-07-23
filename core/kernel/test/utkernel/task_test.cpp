// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "impl/fake_task.h"
#include "task.h"
}

namespace {
const int kDummyParameter = 1234;
bool task_ran;
void* given_parameters;
void FakeTask(void* parameters) {
  given_parameters = parameters;
  task_ran = true;
}
}  // namespace

class TaskTest : public ::testing::Test {
 protected:
  Task instance;

  virtual void SetUp() {
    fake_task_init();
    task_ran = false;
    given_parameters = NULL;
    instance = Task_Create(FakeTask, kTpDefaultPriority, 1024,
                           (void*)&kDummyParameter);
  }
  virtual void TearDown() { Task_Destroy(&instance); }
};

TEST_F(TaskTest, Create) {
  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ(TA_HLNG | TA_RNG0, fake_task_getAttribute(1));
  EXPECT_EQ(9, fake_task_getPriority(1));
  EXPECT_EQ(1024, fake_task_getStackSize(1));
  EXPECT_EQ(TTS_RDY, fake_task_getState(1));
}

TEST_F(TaskTest, CreateThenDispatch) {
  EXPECT_TRUE(fake_task_dispatch(1));
  EXPECT_TRUE(task_ran);
  EXPECT_EQ((void*)&kDummyParameter, given_parameters);
  EXPECT_EQ(TTS_NONE, fake_task_getState(1));  // After end of task
}

TEST_F(TaskTest, CreateWithBoundaryPriority) {
  Task highest_priority = Task_Create(FakeTask, kTpHighestPriority, 128, NULL);
  Task lowest_priority = Task_Create(FakeTask, kTpLowestPriority, 128, NULL);

  EXPECT_EQ(5, fake_task_getPriority(2));
  EXPECT_EQ(12, fake_task_getPriority(3));
  Task_Destroy(&highest_priority);
  Task_Destroy(&lowest_priority);
}

TEST_F(TaskTest, CreateWithNullEntry) {
  EXPECT_EQ(NULL, Task_Create(NULL, kTpDefaultPriority, 1024,
                              (void*)&kDummyParameter));
}

TEST_F(TaskTest, CreateWithOutOfRangePriority) {
  EXPECT_EQ(NULL, Task_Create(FakeTask, kTpLowestPriority - 1, 128, NULL));
  EXPECT_EQ(NULL, Task_Create(FakeTask, kTpHighestPriority + 1, 128, NULL));
}

TEST_F(TaskTest, CreateWithOutOfRangeStackSize) {
  EXPECT_EQ(NULL, Task_Create(FakeTask, kTpDefaultPriority, 0, NULL));
  EXPECT_EQ(NULL, Task_Create(FakeTask, kTpDefaultPriority, -1024, NULL));
  EXPECT_EQ(NULL,
            Task_Create(FakeTask, kTpDefaultPriority, kTssMaxSize + 1, NULL));
}

TEST_F(TaskTest, Destroy) {
  Task_Destroy(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_EQ(TTS_NONE, fake_task_getState(1));
}

TEST_F(TaskTest, DestroyRunningTask) {
  fake_task_setState(1, TTS_RUN);

  Task_Destroy(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_EQ(TTS_NONE, fake_task_getState(1));
}

TEST_F(TaskTest, DestroyAfterDestroy) {
  Task_Destroy(&instance);
  Task_Destroy(&instance);

  SUCCEED();
}

TEST_F(TaskTest, DestroyWithNull) {
  Task_Destroy(NULL);

  SUCCEED();
}

TEST_F(TaskTest, Suspend) {
  Task_Suspend(instance);

  EXPECT_EQ(TTS_SUS, fake_task_getState(1));
  EXPECT_EQ(1, fake_task_getSuspendCount(1));
}

TEST_F(TaskTest, SuspendMoreThanOnce) {
  Task_Suspend(instance);
  Task_Suspend(instance);
  Task_Suspend(instance);

  EXPECT_EQ(TTS_SUS, fake_task_getState(1));
  EXPECT_EQ(1, fake_task_getSuspendCount(1));
}

TEST_F(TaskTest, SuspendRunningTask) {
  fake_task_setState(1, TTS_RUN);

  Task_Suspend(instance);

  EXPECT_EQ(TTS_WAI, fake_task_getState(1));
  EXPECT_EQ(TMO_FEVR, fake_task_getTimeout(1));
  EXPECT_EQ(0, fake_task_getSuspendCount(1));
}

TEST_F(TaskTest, SuspendAfterSuspendedRunningTask) {
  fake_task_setState(1, TTS_RUN);

  Task_Suspend(instance);
  Task_Suspend(instance);

  EXPECT_EQ(TTS_WAI, fake_task_getState(1));
  EXPECT_EQ(TMO_FEVR, fake_task_getTimeout(1));
  EXPECT_EQ(0, fake_task_getSuspendCount(1));
}

TEST_F(TaskTest, SuspendBlockedTask) {
  fake_task_setState(1, TTS_WAI);

  Task_Suspend(instance);

  EXPECT_EQ(TTS_SUS, fake_task_getState(1));
  EXPECT_EQ(1, fake_task_getSuspendCount(1));
}

TEST_F(TaskTest, SuspendWithNull) {
  Task_Suspend(NULL);

  SUCCEED();
}

TEST_F(TaskTest, Resume) {
  Task_Suspend(instance);

  Task_Resume(instance);

  EXPECT_EQ(TTS_RDY, fake_task_getState(1));
  EXPECT_EQ(0, fake_task_getSuspendCount(1));
  EXPECT_EQ(0, fake_task_getWakeupCount(1));
}

TEST_F(TaskTest, ResumeMoreThanOnce) {
  Task_Suspend(instance);

  Task_Resume(instance);
  Task_Resume(instance);
  Task_Resume(instance);

  EXPECT_EQ(TTS_RDY, fake_task_getState(1));
  EXPECT_EQ(0, fake_task_getSuspendCount(1));
  EXPECT_EQ(0, fake_task_getWakeupCount(1));
}

TEST_F(TaskTest, ResumeAfterSuspendedRunningTask) {
  fake_task_setState(1, TTS_RUN);
  Task_Suspend(instance);

  Task_Resume(instance);

  EXPECT_EQ(TTS_RDY, fake_task_getState(1));
  EXPECT_EQ(0, fake_task_getSuspendCount(1));
  EXPECT_EQ(0, fake_task_getWakeupCount(1));
}

TEST_F(TaskTest, ResumeBeforeSuspend) {
  Task_Resume(instance);

  EXPECT_EQ(TTS_RDY, fake_task_getState(1));
  EXPECT_EQ(0, fake_task_getSuspendCount(1));
}

TEST_F(TaskTest, ResumeBeforeSuspendRunningTask) {
  fake_task_setState(1, TTS_RUN);

  Task_Resume(instance);

  EXPECT_EQ(TTS_RUN, fake_task_getState(1));
  EXPECT_EQ(0, fake_task_getSuspendCount(1));
}

TEST_F(TaskTest, ResumeWithNull) {
  Task_Resume(NULL);

  SUCCEED();
}

TEST_F(TaskTest, Delay) {
  fake_task_setState(1, TTS_RUN);

  Task_Delay(instance, 100);

  EXPECT_EQ(TTS_WAI, fake_task_getState(1));
  EXPECT_EQ(100, fake_task_getTimeout(1));
}

TEST_F(TaskTest, DelayNotRunningTask) {
  fake_task_setState(1, TTS_RUN);
  Task task = Task_Create(FakeTask, kTpHighestPriority, 128, NULL);

  Task_Delay(task, 100);

  EXPECT_EQ(TTS_RUN, fake_task_getState(1));
  EXPECT_EQ(0, fake_task_getTimeout(1));
  EXPECT_NE(TTS_WAI, fake_task_getState(2));
  EXPECT_EQ(0, fake_task_getTimeout(2));
}

TEST_F(TaskTest, DelayWithNull) {
  Task_Delay(NULL, 100);

  SUCCEED();
}

TEST_F(TaskTest, DelayWithTimeZeroOrLess) {
  fake_task_setState(1, TTS_RUN);

  Task_Delay(instance, -10);
  Task_Delay(instance, 0);

  EXPECT_EQ(TTS_RUN, fake_task_getState(1));
  EXPECT_EQ(0, fake_task_getTimeout(1));
}
