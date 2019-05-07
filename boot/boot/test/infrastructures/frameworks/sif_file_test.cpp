// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "common/modular_sum.h"
#include "common/spy_runtime_error.h"
#include "infrastructures/frameworks/sif_file.h"
#include "infrastructures/frameworks/sif_header.h"
}

namespace {
SifHeaderStruct dummy_data_file = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStData,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    0,
    0,
    0,
    0x0000000100000000,
    sizeof(SifHeaderStruct),
    0,
    reinterpret_cast<uintptr_t>(&dummy_data_file),
    0,
    0,
    0,
    "This is a dummy data file.",
};

bool was_ran;
int Execute(int runlevel) {
  was_ran = true;
  return runlevel;
}

SifHeaderStruct dummy_exe_file = {
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
    reinterpret_cast<uintptr_t>(&dummy_exe_file),
    reinterpret_cast<uintptr_t>(&Execute),
    0,
    0,
    "This is a dummy exe file.",
};

const char* called_function;
int Called(const char* name, int to_return) {
  called_function = name;
  return to_return;
}
int Open(void) { return Called("Open", 0); }
void Close(void) { Called("Close", 0); }
int Write(int id, const void* data) { return Called("Write", 0); }
int Read(int id, void* data) { return Called("Read", 0); }
int Control(int id, void* data) { return Called("Control", 0); }
uintptr_t primary_api[] = {
    reinterpret_cast<uintptr_t>(Open),    reinterpret_cast<uintptr_t>(Close),
    reinterpret_cast<uintptr_t>(Write),   reinterpret_cast<uintptr_t>(Read),
    reinterpret_cast<uintptr_t>(Control),
};

SifHeaderStruct dummy_lib_file = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStLib,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    5,
    0,
    0,
    0x0000000100000000,
    sizeof(SifHeaderStruct),
    0,
    reinterpret_cast<uintptr_t>(&dummy_lib_file),
    0,
    reinterpret_cast<uintptr_t>(&primary_api),
    0,
    "This is a dummy lib file.",
};

int OpenWithError(void) { return Called("OpenWithError", 1); }
uintptr_t primary_api_with_open_error[] = {
    reinterpret_cast<uintptr_t>(OpenWithError),
};

SifHeaderStruct dummy_open_error_file = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStLib,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    1,
    0,
    0,
    0x0000000100000000,
    sizeof(SifHeaderStruct),
    0,
    reinterpret_cast<uintptr_t>(&dummy_open_error_file),
    0,
    reinterpret_cast<uintptr_t>(&primary_api_with_open_error),
    0,
    "This is a dummy file with open error.",
};

SifHeaderStruct invalid_file = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvNone},
    kStData,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    0,
    0,
    0,
    0x0000000100000000,
    sizeof(SifHeaderStruct),
    0,
    reinterpret_cast<uintptr_t>(&invalid_file),
    0,
    0,
    0,
    "The SIF version is invalid.",
};
}  // namespace

class SifFileTest : public ::testing::Test {
 protected:
  SifFile instance;

  static void SetUpTestCase() {
    SifHeader files[] = {
        &dummy_data_file,       &dummy_exe_file, &dummy_lib_file,
        &dummy_open_error_file, &invalid_file,
    };
    int num_of_files = sizeof(files) / sizeof(files[0]);
    for (int i = 0; i < num_of_files; ++i)
      files[i]->checksum = ModularSum_Calculate(
          reinterpret_cast<uint32_t*>(files[i]->file_address),
          files[i]->file_size);
  }

  virtual void SetUp() {
    called_function = "None";
    was_ran = false;
    instance = NULL;
    SpyRuntimeError_Reset();
  }

  virtual void TearDown() { SifFile_Close(&instance); }
};

TEST_F(SifFileTest, OpenFileHasNoOpenFunction) {
  instance = SifFile_Open(dummy_data_file.file_address);

  ASSERT_TRUE(instance != NULL);
}

TEST_F(SifFileTest, OpenFileHasOpenFunction) {
  instance = SifFile_Open(dummy_lib_file.file_address);

  ASSERT_TRUE(instance != NULL);
  EXPECT_STREQ("Open", called_function);
}

TEST_F(SifFileTest, OpenFileHasOpenFunctionWithError) {
  instance = SifFile_Open(dummy_open_error_file.file_address);

  ASSERT_EQ(NULL, instance);
  EXPECT_STREQ("OpenWithError", called_function);
}

TEST_F(SifFileTest, OpenInvalidFile) {
  instance = SifFile_Open(invalid_file.file_address);

  ASSERT_EQ(NULL, instance);
}

TEST_F(SifFileTest, CloseFileHasNoCloseFunction) {
  instance = SifFile_Open(dummy_data_file.file_address);

  SifFile_Close(&instance);

  EXPECT_EQ(NULL, instance);
}

TEST_F(SifFileTest, CloseFileHasCloseFunction) {
  instance = SifFile_Open(dummy_lib_file.file_address);

  SifFile_Close(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_STREQ("Close", called_function);
}

TEST_F(SifFileTest, CloseWithNull) {
  SifFile_Close(NULL);

  SUCCEED();
}

TEST_F(SifFileTest, CloseMoreThanOnce) {
  instance = SifFile_Open(dummy_lib_file.file_address);

  SifFile_Close(&instance);
  SifFile_Close(&instance);

  EXPECT_EQ(NULL, instance);
}

TEST_F(SifFileTest, GetVersion) {
  instance = SifFile_Open(dummy_data_file.file_address);

  ASSERT_EQ(0x0000000100000000, SifFile_GetVersion(instance));
}

TEST_F(SifFileTest, GetVersionWithNull) {
  ASSERT_EQ(0, SifFile_GetVersion(NULL));
  EXPECT_STREQ("SIF File: null instance", SpyRuntimeError_GetLastError());
}

TEST_F(SifFileTest, ExecuteWithFileHasFunction) {
  instance = SifFile_Open(dummy_exe_file.file_address);

  EXPECT_EQ(3, SifFile_Execute(instance, 3));
  EXPECT_TRUE(was_ran);
}

TEST_F(SifFileTest, ExecuteWithFileHasNoFunction) {
  instance = SifFile_Open(dummy_data_file.file_address);

  EXPECT_EQ(0, SifFile_Execute(instance, 3));
}

TEST_F(SifFileTest, ExecuteWithNull) {
  EXPECT_EQ(0, SifFile_Execute(NULL, 3));
  EXPECT_STREQ("SIF File: null instance", SpyRuntimeError_GetLastError());
}

TEST_F(SifFileTest, WriteWithFileHasFunction) {
  instance = SifFile_Open(dummy_lib_file.file_address);

  EXPECT_EQ(0, SifFile_Write(instance, 0, NULL));
  EXPECT_STREQ("Write", called_function);
}

TEST_F(SifFileTest, WriteWithFileHasNoFunction) {
  instance = SifFile_Open(dummy_data_file.file_address);

  EXPECT_EQ(0, SifFile_Write(instance, 0, NULL));
}

TEST_F(SifFileTest, WriteWithNull) {
  EXPECT_EQ(0, SifFile_Write(NULL, 0, NULL));
  EXPECT_STREQ("SIF File: null instance", SpyRuntimeError_GetLastError());
}

TEST_F(SifFileTest, ReadWithFileHasFunction) {
  instance = SifFile_Open(dummy_lib_file.file_address);

  EXPECT_EQ(0, SifFile_Read(instance, 0, NULL));
  EXPECT_STREQ("Read", called_function);
}

TEST_F(SifFileTest, ReadWithFileHasNoFunction) {
  instance = SifFile_Open(dummy_data_file.file_address);

  EXPECT_EQ(0, SifFile_Read(instance, 0, NULL));
}

TEST_F(SifFileTest, ReadWithNull) {
  EXPECT_EQ(0, SifFile_Read(NULL, 0, NULL));
  EXPECT_STREQ("SIF File: null instance", SpyRuntimeError_GetLastError());
}

TEST_F(SifFileTest, ControlWithFileHasFunction) {
  instance = SifFile_Open(dummy_lib_file.file_address);

  EXPECT_EQ(0, SifFile_Control(instance, 0, NULL));
  EXPECT_STREQ("Control", called_function);
}

TEST_F(SifFileTest, ControlWithFileHasNoFunction) {
  instance = SifFile_Open(dummy_data_file.file_address);

  EXPECT_EQ(0, SifFile_Control(instance, 0, NULL));
}

TEST_F(SifFileTest, ControlWithNull) {
  EXPECT_EQ(0, SifFile_Control(NULL, 0, NULL));
  EXPECT_STREQ("SIF File: null instance", SpyRuntimeError_GetLastError());
}
