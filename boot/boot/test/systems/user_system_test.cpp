// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "modular_sum.h"
#include "sif_header.h"
#include "system_file_entry_point.h"
#include "systems/user_system.h"
#include "systems/user_system_file_provider.h"
}

namespace {
bool was_ran;
int Execute(int unused) {
  was_ran = true;
  return 0;
}
SystemFileEntryPointStruct entry_point = {
    Execute,
};
SifHeaderStruct dummy_file = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStExe,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    {0},
    0x0000000100000000,
    sizeof(SifHeaderStruct),
    0,
    reinterpret_cast<uintptr_t>(&dummy_file),
    0,
    0,
    reinterpret_cast<uintptr_t>(&entry_point),
    "This is a dummy file.",
};
SifHeaderStruct dummy_bad_file;
uintptr_t getFileAddress(int runlevel) {
  if (runlevel != 3) return reinterpret_cast<uintptr_t>(&dummy_bad_file);
  return dummy_file.file_address;
}
IUserSystemFileProviderStruct user_system_file_provider = {
    getFileAddress,
};
}  // namespace

class UserSystemTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    dummy_file.checksum = ModularSum_Calculate(
        reinterpret_cast<uint32_t*>(&dummy_file), dummy_file.file_size);
  }

  virtual void SetUp() { was_ran = false; }
  virtual void TearDown() { UserSystem_Destroy(); }
};

TEST_F(UserSystemTest, GetExecutor) {
  UserSystem_Create(&user_system_file_provider);

  ISystemExecutable instance = UserSystem_getExecutor(3);

  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ(instance, UserSystem_getExecutor(3));
}

TEST_F(UserSystemTest, GetExecutorBeforeCreate) {
  EXPECT_EQ(NULL, UserSystem_getExecutor(3));
}

TEST_F(UserSystemTest, Execute) {
  UserSystem_Create(&user_system_file_provider);

  ISystemExecutable instance = UserSystem_getExecutor(3);

  EXPECT_FALSE(was_ran);
  EXPECT_EQ(0, instance->Execute());
  EXPECT_TRUE(was_ran);
}

TEST_F(UserSystemTest, ExecuteIfNotGetEntryPoint) {
  UserSystem_Create(&user_system_file_provider);

  ISystemExecutable instance = UserSystem_getExecutor(4);

  EXPECT_EQ(-1, instance->Execute());
  EXPECT_FALSE(was_ran);
}
