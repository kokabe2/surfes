// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

#include "mock_io_data.h"

class MockIoDataTest : public ::testing::Test {
 protected:
  virtual void SetUp() { MockIoData_Create(2); }
  virtual void TearDown() { MockIoData_Destroy(); }
};

TEST_F(MockIoDataTest, CanMatchExpectation) {
  MockIoData_ExpectReadThenReturn(1, 2);
  MockIoData_ExpectWrite(1, 2);

  IoData_Write8bit(1, IoData_Read8bit(1));

  SUCCEED();
}

TEST_F(MockIoDataTest, CreateWithCountZeroOrLess) {
  MockIoData_Destroy();

  MockIoData_Create(0);

  EXPECT_FATAL_FAILURE(MockIoData_ExpectWrite(0, 0),
                       "MockIoData is not created, call MockIoData_Create()");
}

TEST_F(MockIoDataTest, ExpectWriteBeforeCreate) {
  MockIoData_Destroy();
  EXPECT_FATAL_FAILURE(MockIoData_ExpectWrite(0, 0),
                       "MockIoData is not created, call MockIoData_Create()");
}

TEST_F(MockIoDataTest, ExpectReadBeforeCreate) {
  MockIoData_Destroy();
  EXPECT_FATAL_FAILURE(MockIoData_ExpectReadThenReturn(0, 0),
                       "MockIoData is not created, call MockIoData_Create()");
}

TEST_F(MockIoDataTest, TooManyWriteExpectations) {
  MockIoData_ExpectWrite(0, 1);
  MockIoData_ExpectWrite(0, 1);
  EXPECT_FATAL_FAILURE(MockIoData_ExpectWrite(0, 1),
                       "MockIoData_ExpectWrite: Too many expectations");
}

TEST_F(MockIoDataTest, TooManyReadExpectations) {
  MockIoData_ExpectReadThenReturn(0, 0);
  MockIoData_ExpectReadThenReturn(0, 0);
  EXPECT_FATAL_FAILURE(
      MockIoData_ExpectReadThenReturn(0, 0),
      "MockIoData_ExpectReadThenReturn: Too many expectations");
}

TEST_F(MockIoDataTest, WriteWhenReadExpected) {
  MockIoData_ExpectReadThenReturn(0, 1);
  EXPECT_FATAL_FAILURE(IoData_Write8bit(0, 0),
                       "Expected IoData_Read(0x0) would return 0x1)\n\t        "
                       "But was IoData_Write(0x0, 0x0)");
}

TEST_F(MockIoDataTest, ReadWhenWriteExpected) {
  MockIoData_ExpectWrite(0, 1);
  EXPECT_FATAL_FAILURE(IoData_Read8bit(0),
                       "Expected IoData_Write(0x0, 0x1)\n\t        "
                       "But was IoData_Read(0x0)");
}

TEST_F(MockIoDataTest, MismatchedWriteAddress) {
  MockIoData_ExpectWrite(0, 0);
  EXPECT_FATAL_FAILURE(IoData_Write8bit(0x10, 0),
                       "Expected IoData_Write(0x0, 0x0)\n\t        "
                       "But was IoData_Write(0x10, 0x0)");
}

TEST_F(MockIoDataTest, MismatchedWriteData) {
  MockIoData_ExpectWrite(0, 0);
  EXPECT_FATAL_FAILURE(IoData_Write8bit(0, 0xde),
                       "Expected IoData_Write(0x0, 0x0)\n\t        "
                       "But was IoData_Write(0x0, 0xde)");
}

TEST_F(MockIoDataTest, MismatchedReadAddress) {
  MockIoData_ExpectReadThenReturn(0x1000, 0xaa);
  EXPECT_FATAL_FAILURE(IoData_Read8bit(0x10000),
                       "Expected IoData_Read(0x1000) returns 0xaa;\n\t        "
                       "But was IoData_Read(0x10000)");
}

TEST_F(MockIoDataTest, TooManyReads) {
  EXPECT_FATAL_FAILURE(IoData_Read8bit(0x10000),
                       "No more expectations but was IoData_Read(0x10000)");
}

TEST_F(MockIoDataTest, TooManyWrites) {
  EXPECT_FATAL_FAILURE(
      IoData_Write8bit(0x10000, 0x12),
      "No more expectations but was IoData_Write(0x10000, 0x12)");
}

TEST_F(MockIoDataTest, NotAllExpectationsUsed) {
  MockIoData_ExpectReadThenReturn(0x1000, 0xaa);
  MockIoData_ExpectWrite(0x1000, 0x55);
  EXPECT_FATAL_FAILURE(MockIoData_VerifyCompletion(),
                       "Expected 2 function(s) used but got 0");
}
