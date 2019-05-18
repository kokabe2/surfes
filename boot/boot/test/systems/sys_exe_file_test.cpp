// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "filer/sif_header.h"
#include "systems/sys_exe_file.h"
#include "modular_sum.h"
}

namespace {
bool was_ran;
int got_parameter;
int Execute(int parameter) {
  was_ran = true;
  got_parameter = parameter;
  return 0;
}

SifHeaderStruct dummy_file_template = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStExe,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    {0},
    0x0000000100000000,
    sizeof(SifHeaderStruct),
    0,
    0,
    0,
    0,
    reinterpret_cast<uintptr_t>(&Execute),
    "This file is for the tests.",
};
}  // namespace

class SysExeFileTest : public ::testing::Test {
 protected:
  SifHeaderStruct dummy_file;
  SysExeFile instance;

  virtual void SetUp() {
    instance = NULL;
    was_ran = false;
    got_parameter = 0;
    dummy_file = dummy_file_template;
    dummy_file.file_address = reinterpret_cast<uintptr_t>(&dummy_file);
    UpdateChecksum();
  }

  virtual void TearDown() { SysExeFile_Close(&instance); }

  void UpdateChecksum() {
    dummy_file.checksum = 0;
    dummy_file.checksum = ModularSum_Calculate(
        reinterpret_cast<uint32_t*>(&dummy_file), dummy_file.file_size);
  }
};

TEST_F(SysExeFileTest, Execute) {
  instance = SysExeFile_Open(dummy_file.file_address);
  EXPECT_FALSE(was_ran);

  int runlevel = SysExeFile_Execute(instance, 1);

  EXPECT_TRUE(was_ran);
  EXPECT_EQ(1, got_parameter);
  EXPECT_EQ(0, runlevel);
}

TEST_F(SysExeFileTest, ExecuteWithNull) {
  int runlevel = SysExeFile_Execute(NULL, 1);

  EXPECT_FALSE(was_ran);
  EXPECT_EQ(0, runlevel);
}

TEST_F(SysExeFileTest, OpenNonExeFile) {
  dummy_file.type = kStLib;
  UpdateChecksum();

  instance = SysExeFile_Open(dummy_file.file_address);

  EXPECT_EQ(NULL, instance);
}

TEST_F(SysExeFileTest, OpenExeFileHasNoEntryPoint) {
  dummy_file.entry_point = 0;
  UpdateChecksum();

  instance = SysExeFile_Open(dummy_file.file_address);

  EXPECT_EQ(NULL, instance);
}
