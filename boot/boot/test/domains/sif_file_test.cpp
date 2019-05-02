// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "common/modular_sum.h"
#include "domains/sif_file.h"
#include "domains/sif_header.h"
}

namespace {
const char* called_function;
int Called(const char* name, int to_return) {
  called_function = name;
  return to_return;
}
int Open(void) { return Called("Open", 0); }
int OpenWithError(void) { return Called("OpenWithError", 1); }
void Close(void) { Called("Close", 0); }
int Write(int id, const void* data) { return Called("Write", 0); }
int Read(int id, void* data) { return Called("Read", 0); }
int Control(int id, void* data) { return Called("Control", 0); }
uintptr_t primary_api_table_template[] = {
    reinterpret_cast<uintptr_t>(Open),    reinterpret_cast<uintptr_t>(Close),
    reinterpret_cast<uintptr_t>(Write),   reinterpret_cast<uintptr_t>(Read),
    reinterpret_cast<uintptr_t>(Control),
};

bool was_ran;
int Execute(void) {
  was_ran = true;
  return 6;
}

SifHeaderStruct dummy_file_template = {
    {0x7F, 0x53, 0x49, 0x46, kSc64, kSd2Lsb, kSvCurrent},
    kStExe,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    5,
    0,
    0,
    0x0001000000000000,
    sizeof(SifHeaderStruct),
    0,
    0,
    reinterpret_cast<uintptr_t>(&Execute),
    0,
    0,
    "This file is for the tests.",
};
}  // namespace

class SifFileTest : public ::testing::Test {
 protected:
  SifFile instance;
  SifHeaderStruct dummy_file;
  uintptr_t primary_api_table[5];

  virtual void SetUp() {
    called_function = "None";
    was_ran = false;
    instance = NULL;
    InitializeDummyFile();
  }

  virtual void TearDown() { SifFile_Close(&instance); }

  void InitializeDummyFile() {
    dummy_file = dummy_file_template;
    dummy_file.file_address = reinterpret_cast<uintptr_t>(&dummy_file);
    memcpy(primary_api_table, primary_api_table_template,
           sizeof(primary_api_table));
    dummy_file.primary_api_address =
        reinterpret_cast<uintptr_t>(primary_api_table);
    UpdateChecksum();
  }

  void UpdateChecksum() {
    dummy_file.checksum = 0;
    dummy_file.checksum = ModularSum_Calculate(
        reinterpret_cast<uint32_t*>(&dummy_file), dummy_file.file_size);
  }
};

TEST_F(SifFileTest, OpenFile) {
  instance = SifFile_Open(reinterpret_cast<uintptr_t>(&dummy_file));

  ASSERT_TRUE(instance != NULL);
  EXPECT_EQ(208, instance->size);
  EXPECT_EQ(0x0001000000000000, instance->version);
}

TEST_F(SifFileTest, OpenInvalidFile) {
  dummy_file.checksum = 0;

  instance = SifFile_Open(reinterpret_cast<uintptr_t>(&dummy_file));

  ASSERT_EQ(NULL, instance);
}

TEST_F(SifFileTest, OpenFileHasNoFunctions) {
  dummy_file.entry_point = 0;
  dummy_file.primary_api_address = 0;
  UpdateChecksum();

  instance = SifFile_Open(reinterpret_cast<uintptr_t>(&dummy_file));

  EXPECT_EQ(0, instance->Execute());
  EXPECT_EQ(0, instance->Write(0, NULL));
  EXPECT_EQ(0, instance->Read(0, NULL));
  EXPECT_EQ(0, instance->Control(0, NULL));
  EXPECT_STREQ("None", called_function);
}

TEST_F(SifFileTest, OpenFileHasOpenFunction) {
  instance = SifFile_Open(reinterpret_cast<uintptr_t>(&dummy_file));

  EXPECT_STREQ("Open", called_function);
  EXPECT_FALSE(was_ran);
}

TEST_F(SifFileTest, OpenFileHasOpenFunctionWithError) {
  primary_api_table[0] = reinterpret_cast<uintptr_t>(OpenWithError);

  instance = SifFile_Open(reinterpret_cast<uintptr_t>(&dummy_file));

  ASSERT_EQ(NULL, instance);
  EXPECT_STREQ("OpenWithError", called_function);
}

TEST_F(SifFileTest, CloseFile) {
  primary_api_table[1] = 0;
  instance = SifFile_Open(reinterpret_cast<uintptr_t>(&dummy_file));

  SifFile_Close(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_STREQ("Open", called_function);
}

TEST_F(SifFileTest, CloseFileHasCloseFunction) {
  instance = SifFile_Open(reinterpret_cast<uintptr_t>(&dummy_file));

  SifFile_Close(&instance);

  EXPECT_STREQ("Close", called_function);
}

TEST_F(SifFileTest, CloseWithNull) {
  SifFile_Close(NULL);

  EXPECT_STREQ("None", called_function);
}

TEST_F(SifFileTest, CloseMoreThanOnce) {
  instance = SifFile_Open(reinterpret_cast<uintptr_t>(&dummy_file));

  SifFile_Close(&instance);
  SifFile_Close(&instance);

  EXPECT_EQ(NULL, instance);
}

TEST_F(SifFileTest, Execute) {
  instance = SifFile_Open(reinterpret_cast<uintptr_t>(&dummy_file));

  int runlevel = instance->Execute();

  EXPECT_EQ(6, runlevel);
  EXPECT_TRUE(was_ran);
}

TEST_F(SifFileTest, Write) {
  instance = SifFile_Open(reinterpret_cast<uintptr_t>(&dummy_file));

  int error = instance->Write(0, NULL);

  EXPECT_EQ(0, error);
  EXPECT_STREQ("Write", called_function);
}

TEST_F(SifFileTest, Read) {
  instance = SifFile_Open(reinterpret_cast<uintptr_t>(&dummy_file));

  int error = instance->Read(0, NULL);

  EXPECT_EQ(0, error);
  EXPECT_STREQ("Read", called_function);
}

TEST_F(SifFileTest, Control) {
  instance = SifFile_Open(reinterpret_cast<uintptr_t>(&dummy_file));

  int error = instance->Control(0, NULL);

  EXPECT_EQ(0, error);
  EXPECT_STREQ("Control", called_function);
}
