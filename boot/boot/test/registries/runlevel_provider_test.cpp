// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "mutable_registry.h"
#include "registries/runlevel_provider.h"
}

TEST(RunlevelProviderTest, GetDefaultRunlevel) {
  EXPECT_EQ(kDefaultRunlevel, RunlevelProvider_getDefaultRunlevel());
}
