﻿// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "infrastructures/frameworks/sif_file_validator.h"
#include "modular_sum.h"
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
    0,
    "This file is for the tests.",
};
}  // namespace

class SifFileValidatorTest : public ::testing::Test {
 protected:
  SifHeaderStruct dummy_file;

  virtual void SetUp() {
    dummy_file = kDummyFileTemplate;
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
  EXPECT_EQ(kSfveNoError, SifFileValidator_Validate(dummy_file.file_address));
}

TEST_F(SifFileValidatorTest, ValidateFileHasWrongMagicNumber) {
  dummy_file.identification[kSiiMagicNumber2] = 0;

  EXPECT_EQ(kSfveMagicNumberError,
            SifFileValidator_Validate(dummy_file.file_address));
}

TEST_F(SifFileValidatorTest, ValidateFileHasInvalidClass) {
  dummy_file.identification[kSiiClass] = kScNone;

  EXPECT_EQ(kSfveClassError,
            SifFileValidator_Validate(dummy_file.file_address));
}

TEST_F(SifFileValidatorTest, ValidateFileHasInvalidVersion) {
  dummy_file.identification[kSiiVersion] = kSvNone;

  EXPECT_EQ(kSfveVersionError,
            SifFileValidator_Validate(dummy_file.file_address));
}

TEST_F(SifFileValidatorTest, ValidateFileHasInvalidHeaderSize) {
  dummy_file.header_size = 200;

  EXPECT_EQ(kSfveHeaderSizeError,
            SifFileValidator_Validate(dummy_file.file_address));
}

TEST_F(SifFileValidatorTest, ValidateFileThatFileSizeIsLessThanHeaderSize) {
  dummy_file.file_size = 204;

  EXPECT_EQ(kSfveFileSizeError,
            SifFileValidator_Validate(dummy_file.file_address));
}

TEST_F(SifFileValidatorTest, ValidateImperfectFile) {
  dummy_file.checksum = 0;

  EXPECT_EQ(kSfveChecksumError,
            SifFileValidator_Validate(dummy_file.file_address));
}
