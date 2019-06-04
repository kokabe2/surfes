// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "immutable_registry.h"
#include "modular_sum.h"
#include "registries/user_system_registry_file.h"
#include "sif_header.h"
#include "user_system_registry_file_interface.h"
}

namespace {
bool was_ran;
int ExecuteR3(void) {
  was_ran = true;
  return 0;
}
ISystemExecutableStruct executors[] = {
    NULL, NULL, NULL, ExecuteR3, NULL, NULL, NULL,
};
ISystemExecutable getExecutor(int runlevel) {
  if (runlevel < kLowestRunlevel || runlevel > kHighestRunlevel) return NULL;

  return &executors[runlevel];
}
UserSystemRegistryFileEntryPointStruct entry_point;
SifHeaderStruct dummy_file_template = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStData,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    {0},
    0x0000000100000000,
    sizeof(SifHeaderStruct),
    0,
    0,
    0,
    0,
    reinterpret_cast<uintptr_t>(&entry_point),
    "This is a dummy file.",
};
}  // namespace

class UserSystemRegistryFileTest : public ::testing::Test {
 protected:
  SifHeaderStruct dummy_file;
  UserSystemRegistryFile instance;

  virtual void SetUp() {
    instance = NULL;
    was_ran = false;
    entry_point.getExecutor = getExecutor;
    dummy_file = dummy_file_template;
    dummy_file.file_address = reinterpret_cast<uintptr_t>(&dummy_file);
    UpdateChecksum();
  }

  virtual void TearDown() { UserSystemRegistryFile_Close(&instance); }

  void UpdateChecksum() {
    dummy_file.checksum = 0;
    dummy_file.checksum = ModularSum_Calculate(
        reinterpret_cast<uint32_t*>(&dummy_file), dummy_file.file_size);
  }
};

TEST_F(UserSystemRegistryFileTest, GetExecutor) {
  instance = UserSystemRegistryFile_Open(dummy_file.file_address);

  ISystemExecutable se = UserSystemRegistryFile_getExecutor(instance, 3);

  EXPECT_TRUE(se != NULL);
  EXPECT_EQ(se, UserSystemRegistryFile_getExecutor(instance, 3));
}

TEST_F(UserSystemRegistryFileTest, GetExecutorWithNull) {
  EXPECT_EQ(NULL, UserSystemRegistryFile_getExecutor(NULL, 3));
}

TEST_F(UserSystemRegistryFileTest, OpenNonDataFile) {
  dummy_file.type = kStExe;
  UpdateChecksum();

  instance = UserSystemRegistryFile_Open(dummy_file.file_address);

  EXPECT_EQ(NULL, instance);
}

TEST_F(UserSystemRegistryFileTest, OpenExeFileHasNoEntryPoint) {
  dummy_file.entry_point = 0;
  UpdateChecksum();

  instance = UserSystemRegistryFile_Open(dummy_file.file_address);

  EXPECT_EQ(NULL, instance);
}

TEST_F(UserSystemRegistryFileTest, OpenExeFileHasNoGetExecutor) {
  entry_point.getExecutor = NULL;

  instance = UserSystemRegistryFile_Open(dummy_file.file_address);

  EXPECT_EQ(NULL, instance);
}

TEST_F(UserSystemRegistryFileTest, Execute) {
  instance = UserSystemRegistryFile_Open(dummy_file.file_address);

  ISystemExecutable se = UserSystemRegistryFile_getExecutor(instance, 3);

  EXPECT_FALSE(was_ran);
  EXPECT_EQ(0, se->Execute());
  EXPECT_TRUE(was_ran);
}
