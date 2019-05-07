// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "common/modular_sum.h"
#include "infrastructures/frameworks/core_file.h"
#include "infrastructures/frameworks/sif_header.h"
}

namespace {
int Execute(int runlevel) { return runlevel; }
SifHeaderStruct dummy_core_file = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStExe,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    0,
    0,
    0,
    0x0000000100000000,
    sizeof(SifHeaderStruct),
    0,
    reinterpret_cast<uintptr_t>(&dummy_core_file),
    reinterpret_cast<uintptr_t>(&Execute),
    0,
    0,
    "This is a dummy exe file.",
};
}  // namespace

class CoreFileTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    dummy_core_file.checksum =
        ModularSum_Calculate(reinterpret_cast<uint32_t*>(&dummy_core_file),
                             dummy_core_file.file_size);
  }

  virtual void SetUp() { CoreFile_Create(dummy_core_file.file_address); }

  virtual void TearDown() { CoreFile_Destroy(); }
};

TEST_F(CoreFileTest, GetInstance) {
  SifFile* instance = CoreFile_getInstance();

  EXPECT_EQ(instance, CoreFile_getInstance());
  EXPECT_EQ(3, SifFile_Execute(*instance, 3));
}
