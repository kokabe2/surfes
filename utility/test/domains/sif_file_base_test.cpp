// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "modular_sum.h"
#include "sif_file_base.h"
#include "sif_header.h"
}

namespace {
SifHeaderStruct dummy_file_template = {
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
    0,
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

class SifFileBaseTest : public ::testing::Test {
 protected:
  SifHeaderStruct dummy_file;
  SifFileBase instance;

  virtual void SetUp() {
    called_function = "None";
    instance = NULL;
    dummy_file = dummy_file_template;
    dummy_file.file_address = reinterpret_cast<uintptr_t>(&dummy_file);
    UpdateChecksum();
  }

  void UpdateChecksum() {
    dummy_file.checksum = 0;
    dummy_file.checksum = ModularSum_Calculate(
        reinterpret_cast<uint32_t*>(&dummy_file), dummy_file.file_size);
  }

  virtual void TearDown() { SifFileBase_Close(&instance); }
};

TEST_F(SifFileBaseTest, OpenFileHasNoOpenFunction) {
  instance =
      SifFileBase_Open(dummy_file.file_address, sizeof(SifFileBaseStruct));

  ASSERT_TRUE(instance != NULL);
  EXPECT_STREQ("None", called_function);
}

TEST_F(SifFileBaseTest, OpenFileHasOpenFunction) {
  dummy_file.open_function_address = reinterpret_cast<uintptr_t>(Open);
  UpdateChecksum();

  instance =
      SifFileBase_Open(dummy_file.file_address, sizeof(SifFileBaseStruct));

  ASSERT_TRUE(instance != NULL);
  EXPECT_STREQ("Open", called_function);
}

TEST_F(SifFileBaseTest, OpenFileHasOpenFunctionWithError) {
  dummy_file.open_function_address = reinterpret_cast<uintptr_t>(OpenWithError);
  UpdateChecksum();

  instance =
      SifFileBase_Open(dummy_file.file_address, sizeof(SifFileBaseStruct));

  ASSERT_EQ(NULL, instance);
  EXPECT_STREQ("OpenWithError", called_function);
}

TEST_F(SifFileBaseTest, OpenInvalidFile) {
  dummy_file.checksum = 0;

  instance =
      SifFileBase_Open(dummy_file.file_address, sizeof(SifFileBaseStruct));

  ASSERT_EQ(NULL, instance);
}

TEST_F(SifFileBaseTest, CloseFileHasNoCloseFunction) {
  instance =
      SifFileBase_Open(dummy_file.file_address, sizeof(SifFileBaseStruct));

  SifFileBase_Close(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_STREQ("None", called_function);
}

TEST_F(SifFileBaseTest, CloseFileHasCloseFunction) {
  dummy_file.close_function_address = reinterpret_cast<uintptr_t>(Close);
  UpdateChecksum();

  instance =
      SifFileBase_Open(dummy_file.file_address, sizeof(SifFileBaseStruct));

  SifFileBase_Close(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_STREQ("Close", called_function);
}

TEST_F(SifFileBaseTest, CloseWithNull) {
  SifFileBase_Close(NULL);

  SUCCEED();
}

TEST_F(SifFileBaseTest, CloseMoreThanOnce) {
  instance =
      SifFileBase_Open(dummy_file.file_address, sizeof(SifFileBaseStruct));

  SifFileBase_Close(&instance);
  SifFileBase_Close(&instance);

  EXPECT_EQ(NULL, instance);
}

TEST_F(SifFileBaseTest, GetVersion) {
  instance =
      SifFileBase_Open(dummy_file.file_address, sizeof(SifFileBaseStruct));

  ASSERT_EQ(0x0000000100000000, SifFileBase_getVersion(instance));
}

TEST_F(SifFileBaseTest, GetVersionWithNull) {
  ASSERT_EQ(0, SifFileBase_getVersion(NULL));
}
