// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "modular_sum.h"
#include "mutable_registry.h"
#include "registry_api.h"
#include "registry_api_private.h"
#include "sif_header.h"
}

namespace {
int given_key;
bool get_int_called;
int getIntRegistryValue(int key) {
  get_int_called = true;
  given_key = key;
  return 0;
}
RegistryApiStruct registry_api = {
    getIntRegistryValue, nullptr,
};
SifHeaderStruct dummy_file = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStData,
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
    reinterpret_cast<uintptr_t>(&registry_api),
    "This file is for the tests.",
};

SifHeaderStruct dummy_broken_file = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStData,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    {0},
    0x0000000100000000,
    sizeof(SifHeaderStruct),
    0,
    reinterpret_cast<uintptr_t>(&dummy_broken_file),
    0,
    0,
    0,
    "This file is for the tests.",
};
}  // namespace

class RegistryApiTest : public ::testing::Test {
 protected:
  RegistryApi instance;

  static void SetUpTestCase() {
    dummy_file.checksum = ModularSum_Calculate(
        reinterpret_cast<uint32_t*>(&dummy_file), dummy_file.file_size);
  }

  virtual void SetUp() {
    get_int_called = false;
    given_key = -1;
  }

  virtual void TearDown() { RegistryApi_Destroy(); }
};

TEST_F(RegistryApiTest, GetInstanceWithRegistryFile) {
  RegistryApi_Create(dummy_file.file_address);

  instance = RegistryApi_getInstance();

  EXPECT_FALSE(get_int_called);
  EXPECT_EQ(instance, RegistryApi_getInstance());
  EXPECT_EQ(0, instance->getIntRegistryValue(kIrkDefaultRunlevel));
  EXPECT_EQ(kIrkDefaultRunlevel, given_key);
  EXPECT_TRUE(get_int_called);
}

TEST_F(RegistryApiTest, GetInstanceWithBuiltinData) {
  RegistryApi_Create(reinterpret_cast<uintptr_t>(&dummy_broken_file));

  instance = RegistryApi_getInstance();

  EXPECT_EQ(instance, RegistryApi_getInstance());
  EXPECT_EQ(kDefaultRunlevel,
            instance->getIntRegistryValue(kIrkDefaultRunlevel));
}
