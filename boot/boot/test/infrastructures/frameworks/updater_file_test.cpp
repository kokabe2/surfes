// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "common/modular_sum.h"
#include "infrastructures/frameworks/sif_header.h"
#include "infrastructures/frameworks/updater_file.h"
}

namespace {
int Execute(int runlevel) { return runlevel; }
SifHeaderStruct dummy_updater_file = {
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
    reinterpret_cast<uintptr_t>(&dummy_updater_file),
    reinterpret_cast<uintptr_t>(&Execute),
    0,
    0,
    "This is a dummy exe file.",
};
}  // namespace

class UpdaterFileTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    dummy_updater_file.checksum =
        ModularSum_Calculate(reinterpret_cast<uint32_t*>(&dummy_updater_file),
                             dummy_updater_file.file_size);
  }

  virtual void SetUp() { UpdaterFile_Create(dummy_updater_file.file_address); }

  virtual void TearDown() { UpdaterFile_Destroy(); }
};

TEST_F(UpdaterFileTest, GetInstance) {
  SifFile* instance = UpdaterFile_getInstance();

  EXPECT_EQ(instance, UpdaterFile_getInstance());
  EXPECT_EQ(1, SifFile_Execute(*instance, 1));
}
