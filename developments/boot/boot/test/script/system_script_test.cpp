// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include <stdio.h>

#include "gtest/gtest.h"

extern "C" {
#include "script/system_executable.h"
#include "script/system_executor_factory.h"
#include "script/system_script.h"
}

namespace {
char executed_log[256];
int log_offset;
void ClearLog(void) {
  log_offset = 0;
  snprintf(executed_log, sizeof(executed_log), "%s", "None");
}
void AddLog(int runlevel) {
  char format[] = "(%d) was ran\n";
  log_offset += snprintf(executed_log + log_offset,
                         sizeof(executed_log) - log_offset, format, runlevel);
}

int next_runlevels[7];
const int kNextRunlevels[] = {
    -1, 6, -1, 0, -1, -1, -1,
};
int Execute(int runlevel) {
  AddLog(runlevel);
  return next_runlevels[runlevel];
}
int ExecuteR0(void) { return Execute(0); }
int ExecuteR1(void) { return Execute(1); }
int ExecuteR3(void) { return Execute(3); }
int ExecuteR6(void) { return Execute(6); }
ISystemExecutableStruct dummy_executors[] = {
    ExecuteR0, ExecuteR1, nullptr, ExecuteR3, nullptr, nullptr, ExecuteR6,
};

ISystemExecutable Make(int runlevel) {
  if (runlevel < 0 || runlevel > 6) return nullptr;

  return &dummy_executors[runlevel];
}
ISystemExecutorFactoryStruct factory = {
    Make,
};
}  // namespace

class SystemRunnerTest : public ::testing::Test {
 protected:
  int default_runlevel;

  virtual void SetUp() {
    SystemScript_Create(&factory);
    ClearLog();
    default_runlevel = 3;
    memcpy(next_runlevels, kNextRunlevels, sizeof(next_runlevels));
  }

  virtual void TearDown() { SystemScript_Destroy(); }
};

TEST_F(SystemRunnerTest, RunDefaultScript) {
  SystemScript_Run(default_runlevel);

  EXPECT_STREQ(executed_log, "(3) was ran\n(0) was ran\n");
}

TEST_F(SystemRunnerTest, RunUpdateScript) {
  next_runlevels[3] = 1;

  SystemScript_Run(default_runlevel);

  EXPECT_STREQ(executed_log, "(3) was ran\n(1) was ran\n(6) was ran\n");
}

TEST_F(SystemRunnerTest, RunRecoveryScript) {
  default_runlevel = 1;
  next_runlevels[1] = 3;

  SystemScript_Run(default_runlevel);

  EXPECT_STREQ(executed_log, "(1) was ran\n(3) was ran\n(0) was ran\n");
}

TEST_F(SystemRunnerTest, RunWithInvalidRunlevel) {
  default_runlevel = 7;

  SystemScript_Run(default_runlevel);

  EXPECT_STREQ(executed_log, "None");
}

TEST_F(SystemRunnerTest, RunBeforeCreateOrAfterDestroy) {
  SystemScript_Destroy();

  SystemScript_Run(default_runlevel);

  EXPECT_STREQ(executed_log, "None");
}
