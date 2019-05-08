// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include <stdio.h>

#include "gtest/gtest.h"

extern "C" {
#include "common/modular_sum.h"
#include "infrastructures/frameworks/core_file.h"
#include "infrastructures/frameworks/sif_header.h"
#include "infrastructures/frameworks/system_run_service_interface.h"
#include "infrastructures/frameworks/system_runner.h"
#include "infrastructures/frameworks/updater_file.h"
}

namespace {
char executed_log[256];
int log_offset;
void ClearLog(void) {
  log_offset = 0;
  snprintf(executed_log, sizeof(executed_log), "%s", "None");
}
void AddLog(const char* message, int runlevel) {
  char format[] = "%s (%d)\n";
  log_offset +=
      snprintf(executed_log + log_offset, sizeof(executed_log) - log_offset,
               format, message, runlevel);
}

int core_return;
int ExecuteCore(int runlevel) {
  AddLog("Core was ran", runlevel);
  return core_return;
}
SifHeaderStruct dummy_core_file = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStExe,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    0,
    0,
    0,
    0x0000000100000000,
    sizeof(SifHeaderStruct),
    0,
    reinterpret_cast<uintptr_t>(&dummy_core_file),
    reinterpret_cast<uintptr_t>(&ExecuteCore),
    0,
    0,
    "This is a dummy exe file.",
};

int ExecuteUpdater(int runlevel) {
  AddLog("Updater was ran", runlevel);
  return 6;
}
SifHeaderStruct dummy_updater_file = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStExe,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    0,
    0,
    0,
    0x0000000100000000,
    sizeof(SifHeaderStruct),
    0,
    reinterpret_cast<uintptr_t>(&dummy_updater_file),
    reinterpret_cast<uintptr_t>(&ExecuteUpdater),
    0,
    0,
    "This is a dummy exe file.",
};
}  // namespace

void SystemRunServiceInterface_Halt(void) { AddLog("Halt", 0); }
void SystemRunServiceInterface_Reboot(void) { AddLog("Reboot", 6); }

class SystemRunnerTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    dummy_core_file.checksum =
        ModularSum_Calculate(reinterpret_cast<uint32_t*>(&dummy_core_file),
                             dummy_core_file.file_size);
    dummy_updater_file.checksum =
        ModularSum_Calculate(reinterpret_cast<uint32_t*>(&dummy_updater_file),
                             dummy_updater_file.file_size);
  }

  virtual void SetUp() {
    CoreFile_Create(dummy_core_file.file_address);
    UpdaterFile_Create(dummy_updater_file.file_address);
    SystemRunner_Create(3);
    ClearLog();
  }

  virtual void TearDown() {
    SystemRunner_Destroy();
    CoreFile_Destroy();
    UpdaterFile_Destroy();
  }
};

TEST_F(SystemRunnerTest, RunNormalSequence) {
  core_return = 0;

  SystemRunner_Run();

  EXPECT_STREQ(executed_log, "Core was ran (3)\nHalt (0)\n");
}

TEST_F(SystemRunnerTest, RunUpdateSequence) {
  core_return = 1;

  SystemRunner_Run();

  EXPECT_STREQ(executed_log,
               "Core was ran (3)\nUpdater was ran (1)\nReboot (6)\nHalt (0)\n");
}

TEST_F(SystemRunnerTest, RunWithInvalidRunlevel) {
  core_return = 7;

  SystemRunner_Run();

  EXPECT_STREQ(executed_log, "Core was ran (3)\nHalt (0)\n");
}
