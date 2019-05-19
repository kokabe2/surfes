// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "modular_sum.h"
#include "registries/user_system_registry.h"
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
UserSystemRegistryFileEntryPointStruct entry_point = {
    getExecutor,
};
SifHeaderStruct dummy_file = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStLib,
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
}  // namespace

class UserSystemRegistryTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    dummy_file.checksum = ModularSum_Calculate(
        reinterpret_cast<uint32_t*>(&dummy_file), dummy_file.file_size);
    UserSystemRegistry_Change(dummy_file.file_address);
  }

  virtual void SetUp() { was_ran = false; }
};

TEST_F(UserSystemRegistryTest, GetExecutor) {
  ISystemExecutable se = UserSystemRegistry_getExecutor(3);

  EXPECT_TRUE(se != NULL);
  EXPECT_EQ(se, UserSystemRegistry_getExecutor(3));
}

TEST_F(UserSystemRegistryTest, Execute) {
  ISystemExecutable se = UserSystemRegistry_getExecutor(3);

  EXPECT_FALSE(was_ran);
  EXPECT_EQ(0, se->Execute());
  EXPECT_TRUE(was_ran);
}
