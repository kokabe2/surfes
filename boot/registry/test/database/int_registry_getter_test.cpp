// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "database/int_registry_getter.h"
#include "spy_runtime_error.h"
}

TEST(IntRegistryGetterTest, GetWithNonExistKey) {
  SpyRuntimeError_Reset();

  EXPECT_EQ(-1, IntRegistryGetter_get(-3));
  EXPECT_EQ(-1, IntRegistryGetter_get(412037413));
  EXPECT_STREQ("Int Registry Getter: out-of-range key",
               SpyRuntimeError_GetLastError());
  EXPECT_EQ(412037413, SpyRuntimeError_GetLastParameter());
}
