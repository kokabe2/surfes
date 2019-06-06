// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "systems/reboot_system.h"
#include "systems/reboot_system_service_interface.h"
}

namespace {
bool was_ran;
}  // namespace

void RebootSystemServiceInterface_Reboot(void) { was_ran = true; }

class RebootSystemTest : public ::testing::Test {
 protected:
  ISystemExecutable instance;

  virtual void SetUp() {
    was_ran = false;
    instance = RebootSystem_getInstance();
  }
};

TEST_F(RebootSystemTest, GetInstance) {
  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ(instance, RebootSystem_getInstance());
}

TEST_F(RebootSystemTest, Execute) {
  EXPECT_FALSE(was_ran);
  instance->Execute();
  EXPECT_TRUE(was_ran);
}
