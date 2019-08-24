// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "domains/sif_file_validator.h"
#include "modular_sum.h"
#include "sif_header.h"
#include "spy_runtime_error.h"
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
}  // namespace

class SifFileValidatorTest : public ::testing::Test {
 protected:
  SifHeaderStruct dummy_file;

  virtual void SetUp() {
    SpyRuntimeError_Reset();
    dummy_file = dummy_file_template;
    dummy_file.file_address = reinterpret_cast<uintptr_t>(&dummy_file);
    UpdateChecksum();
  }

  void UpdateChecksum() {
    dummy_file.checksum = 0;
    dummy_file.checksum = ModularSum_Calculate(
        reinterpret_cast<uint32_t*>(&dummy_file), dummy_file.file_size);
  }
};

TEST_F(SifFileValidatorTest, ValidateFile) {
  int error = SifFileValidator_Validate(dummy_file.file_address);

  EXPECT_EQ(kSfveNoError, error);
}

TEST_F(SifFileValidatorTest, ValidateFileHasWrongMagicNumber) {
  dummy_file.identification[kSiiMagicNumber2] = 0;

  int error = SifFileValidator_Validate(dummy_file.file_address);

  EXPECT_EQ(kSfveMagicNumberError, error);
  EXPECT_STREQ("SIF File Validator: non SIF file",
               SpyRuntimeError_GetLastError());
  EXPECT_EQ(kSiiMagicNumber2, SpyRuntimeError_GetLastParameter());
}

TEST_F(SifFileValidatorTest, ValidateFileHasInvalidClass) {
  dummy_file.identification[kSiiClass] = kScNone;

  int error = SifFileValidator_Validate(dummy_file.file_address);

  EXPECT_EQ(kSfveClassError, error);
  EXPECT_STREQ("SIF File Validator: invalid SIF class",
               SpyRuntimeError_GetLastError());
  EXPECT_EQ(kScNone, SpyRuntimeError_GetLastParameter());
}

TEST_F(SifFileValidatorTest, ValidateFileHasInvalidVersion) {
  dummy_file.identification[kSiiVersion] = kSvNone;

  int error = SifFileValidator_Validate(dummy_file.file_address);

  EXPECT_EQ(kSfveVersionError, error);
  EXPECT_STREQ("SIF File Validator: invalid SIF version",
               SpyRuntimeError_GetLastError());
  EXPECT_EQ(kSvNone, SpyRuntimeError_GetLastParameter());
}

TEST_F(SifFileValidatorTest, ValidateFileHasInvalidHeaderSize) {
  dummy_file.header_size = 200;

  int error = SifFileValidator_Validate(dummy_file.file_address);

  EXPECT_EQ(kSfveHeaderSizeError, error);
  EXPECT_STREQ("SIF File Validator: invalid header size",
               SpyRuntimeError_GetLastError());
  EXPECT_EQ(200, SpyRuntimeError_GetLastParameter());
}

TEST_F(SifFileValidatorTest, ValidateFileThatFileSizeIsLessThanHeaderSize) {
  dummy_file.file_size = 204;

  int error = SifFileValidator_Validate(dummy_file.file_address);

  EXPECT_EQ(kSfveFileSizeError, error);
  EXPECT_STREQ("SIF File Validator: invalid file size",
               SpyRuntimeError_GetLastError());
  EXPECT_EQ(204, SpyRuntimeError_GetLastParameter());
}

TEST_F(SifFileValidatorTest, ValidateImperfectFile) {
  dummy_file.checksum = 0;

  int error = SifFileValidator_Validate(dummy_file.file_address);

  EXPECT_EQ(kSfveChecksumError, error);
}

TEST_F(SifFileValidatorTest, ValidateFileThatFileSizeIsNotMultiplesOfFour) {
  dummy_file.file_size = 209;
  UpdateChecksum();

  int error = SifFileValidator_Validate(dummy_file.file_address);

  EXPECT_EQ(kSfveChecksumError, error);
}
