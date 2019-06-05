// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "immutable_registry.h"
#include "mutable_registry.h"
#include "registries/user_system_file_provider_impl.h"
}

class UserSystemFileProviderImplTest : public ::testing::Test {
 protected:
  IUserSystemFileProvider instance;

  virtual void SetUp() { instance = UserSystemFileProviderImpl_getInstance(); }
};

TEST_F(UserSystemFileProviderImplTest, GetInstance) {
  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ(instance, UserSystemFileProviderImpl_getInstance());
}

TEST_F(UserSystemFileProviderImplTest, GetFileAddress) {
  EXPECT_EQ(kRecoverySystemFileAddress,
            instance->getFileAddress(kRecoveryModeRunlevel));
  EXPECT_EQ(kUpdateSystemFileAddress,
            instance->getFileAddress(kUpdateModeRunlevel));
  EXPECT_EQ(kNormalSystemFileAddress,
            instance->getFileAddress(kNormalModeRunlevel));
  EXPECT_EQ(kAdvancedSystemFileAddress,
            instance->getFileAddress(kAdvancedModeRunlevel));
}

TEST_F(UserSystemFileProviderImplTest, GetFileAddressWithOutRangeOfRunlevel) {
  EXPECT_EQ(0, instance->getFileAddress(6));
  EXPECT_EQ(0, instance->getFileAddress(0));
  EXPECT_EQ(0, instance->getFileAddress(3141));
  EXPECT_EQ(0, instance->getFileAddress(-1));
}
