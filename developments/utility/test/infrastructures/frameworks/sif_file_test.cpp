// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "modular_sum.h"
#include "sif_file.h"
#include "sif_header.h"
}

namespace {
const SifHeaderStruct kDummyFileTemplate = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStData,
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
    0x0123456789ABCDEF,
    "This file is for the tests.",
};

const char* called_function;
int Called(const char* name, int to_return) {
  called_function = name;
  return to_return;
}
int Open(void) { return Called("Open", 0); }
int OpenWithError(void) { return Called("OpenWithError", 1); }
void Close(void) { Called("Close", 0); }
}  // namespace

class SifFileTest : public ::testing::Test {
 protected:
  SifHeaderStruct dummy_file;
  SifFile instance;

  virtual void SetUp() {
    called_function = "None";
    instance = NULL;
    dummy_file = kDummyFileTemplate;
    dummy_file.file_address = reinterpret_cast<uintptr_t>(&dummy_file);
    UpdateChecksum();
  }

  void UpdateChecksum() {
    dummy_file.checksum = 0;
    dummy_file.checksum = ModularSum_Calculate(
        reinterpret_cast<uint32_t*>(&dummy_file), dummy_file.file_size);
  }

  virtual void TearDown() { SifFile_Close(&instance); }
};

TEST_F(SifFileTest, OpenFileHasNoOpenFunction) {
  instance = SifFile_Open(dummy_file.file_address);

  ASSERT_TRUE(instance != NULL);
  EXPECT_STREQ("None", called_function);
}

TEST_F(SifFileTest, OpenFileHasOpenFunction) {
  dummy_file.open_function_address = reinterpret_cast<uintptr_t>(Open);
  UpdateChecksum();

  instance = SifFile_Open(dummy_file.file_address);

  ASSERT_TRUE(instance != NULL);
  EXPECT_STREQ("Open", called_function);
}

TEST_F(SifFileTest, OpenFileHasOpenFunctionWithError) {
  dummy_file.open_function_address = reinterpret_cast<uintptr_t>(OpenWithError);
  UpdateChecksum();

  instance = SifFile_Open(dummy_file.file_address);

  ASSERT_EQ(NULL, instance);
  EXPECT_STREQ("OpenWithError", called_function);
}

TEST_F(SifFileTest, OpenInvalidFile) {
  dummy_file.checksum = 0;

  instance = SifFile_Open(dummy_file.file_address);

  ASSERT_EQ(NULL, instance);
}

TEST_F(SifFileTest, CloseFileHasNoCloseFunction) {
  instance = SifFile_Open(dummy_file.file_address);

  SifFile_Close(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_STREQ("None", called_function);
}

TEST_F(SifFileTest, CloseFileHasCloseFunction) {
  dummy_file.close_function_address = reinterpret_cast<uintptr_t>(Close);
  UpdateChecksum();
  instance = SifFile_Open(dummy_file.file_address);

  SifFile_Close(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_STREQ("Close", called_function);
}

TEST_F(SifFileTest, CloseWithNull) {
  SifFile_Close(NULL);

  SUCCEED();
}

TEST_F(SifFileTest, CloseAfterClose) {
  instance = SifFile_Open(dummy_file.file_address);

  SifFile_Close(&instance);
  SifFile_Close(&instance);

  SUCCEED();
}

TEST_F(SifFileTest, GetVersion) {
  instance = SifFile_Open(dummy_file.file_address);

  EXPECT_EQ(0x0000000100000000, SifFile_getVersion(instance));
}

TEST_F(SifFileTest, GetVersionWithNull) {
  EXPECT_EQ(0, SifFile_getVersion(NULL));
}

TEST_F(SifFileTest, GetEntryPoint) {
  instance = SifFile_Open(dummy_file.file_address);

  EXPECT_EQ(0x0123456789ABCDEF, SifFile_getEntryPoint(instance));
}

TEST_F(SifFileTest, GetEntryPointWithNull) {
  EXPECT_EQ(0, SifFile_getEntryPoint(NULL));
}
