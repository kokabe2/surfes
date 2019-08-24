// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "systems/halt_system.h"
#include "systems/halt_system_service_interface.h"
}

namespace {
bool was_ran;
}  // namespace

void HaltSystemServiceInterface_Shutdown(void) { was_ran = true; }

class HaltSystemTest : public ::testing::Test {
 protected:
  ISystemExecutable instance;

  virtual void SetUp() {
    was_ran = false;
    instance = HaltSystem_getInstance();
  }
};

TEST_F(HaltSystemTest, GetInstance) {
  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ(instance, HaltSystem_getInstance());
}

TEST_F(HaltSystemTest, Execute) {
  EXPECT_FALSE(was_ran);
  instance->Execute();
  EXPECT_TRUE(was_ran);
}
