// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "systems/system_executor_factory_impl.h"
#include "utilities/spy_runtime_error.h"
}

class SystemExecutorFactoryImplTest : public ::testing::Test {
 protected:
  ISystemExecutorFactory instance;

  virtual void SetUp() {
    instance = SystemExecutorFactoryImpl_getInstance();
    SpyRuntimeError_Reset();
  }
};

TEST_F(SystemExecutorFactoryImplTest, GetInstance) {
  EXPECT_EQ(instance, SystemExecutorFactoryImpl_getInstance());
}

TEST_F(SystemExecutorFactoryImplTest, Make) {
  ISystemExecutable se = instance->Make(0);

  EXPECT_EQ(se, instance->Make(0));
}

TEST_F(SystemExecutorFactoryImplTest, MakeWithOutRangeOfRunlevel) {
  EXPECT_EQ(NULL, instance->Make(7));
  EXPECT_EQ(NULL, instance->Make(-1));
  EXPECT_STREQ("System Executor Factory Impl: out-of-range runlevel",
               SpyRuntimeError_GetLastError());
  EXPECT_EQ(-1, SpyRuntimeError_GetLastParameter());
}
