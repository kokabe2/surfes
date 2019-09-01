// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "diagnosis/memory_diagnostic_transaction.h"
}

class MemoryDiagnosticTransactionTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() { MemoryDiagnosticTransaction_Destroy(); }
};

TEST_F(MemoryDiagnosticTransactionTest, ExecuteTransaction) {
  uint16_t dummy_memory[128] = {0};

  MemoryDiagnosticTransaction_Create((uintptr_t)dummy_memory,
                                     sizeof(dummy_memory), sizeof(uint16_t));
  MemoryDiagnosticTransaction_Add(0x5A5A);
  MemoryDiagnosticTransaction_Add(0x9696);
  bool is_success = MemoryDiagnosticTransaction_Execute();

  EXPECT_TRUE(is_success);
  for (int i = 0; i < 128; ++i)
    ASSERT_EQ(0x9696, dummy_memory[i]) << "Failure at index " << i;
}

TEST_F(MemoryDiagnosticTransactionTest, CreateWithSizeLessThanZero) {
  uint32_t dummy_memory[8] = {0};

  MemoryDiagnosticTransaction_Create((uintptr_t)dummy_memory, -4,
                                     sizeof(uint32_t));
  MemoryDiagnosticTransaction_Add(0x12345678);
  bool is_success = MemoryDiagnosticTransaction_Execute();

  EXPECT_FALSE(is_success);
  for (int i = 0; i < 8; ++i)
    ASSERT_EQ(0, dummy_memory[i]) << "Failure at index " << i;
}

TEST_F(MemoryDiagnosticTransactionTest, CreateWithInvalidBusWidth) {
  uint8_t dummy_memory[8] = {0};

  MemoryDiagnosticTransaction_Create((uintptr_t)dummy_memory,
                                     sizeof(dummy_memory), 3);
  MemoryDiagnosticTransaction_Add(0xC3);
  bool is_success = MemoryDiagnosticTransaction_Execute();

  EXPECT_FALSE(is_success);
  for (int i = 0; i < 8; ++i)
    ASSERT_EQ(0, dummy_memory[i]) << "Failure at index " << i;
}

TEST_F(MemoryDiagnosticTransactionTest, CreateWithMemoryOverflow) {
  MemoryDiagnosticTransaction_Create(UINTPTR_MAX, 1, sizeof(uint8_t));
  MemoryDiagnosticTransaction_Add(0xC3);
  bool is_success = MemoryDiagnosticTransaction_Execute();

  EXPECT_FALSE(is_success);
}

TEST_F(MemoryDiagnosticTransactionTest, CreateMultipleTimesWithoutDestroy) {
  uint32_t dummy_memory1[8] = {0};
  uint16_t dummy_memory2[8] = {0};

  MemoryDiagnosticTransaction_Create((uintptr_t)dummy_memory1,
                                     sizeof(dummy_memory1), sizeof(uint32_t));
  MemoryDiagnosticTransaction_Add(0x12345678);
  MemoryDiagnosticTransaction_Create((uintptr_t)dummy_memory2,
                                     sizeof(dummy_memory2), sizeof(uint16_t));
  MemoryDiagnosticTransaction_Add(0xABCD);
  bool is_success = MemoryDiagnosticTransaction_Execute();

  EXPECT_TRUE(is_success);
  for (int i = 0; i < 8; ++i)
    ASSERT_EQ(0, dummy_memory1[i]) << "Failure at index " << i;
  for (int i = 0; i < 8; ++i)
    ASSERT_EQ(0xABCD, dummy_memory2[i]) << "Failure at index " << i;
}

TEST_F(MemoryDiagnosticTransactionTest, AddZero) {
  uint32_t dummy_memory[8] = {0};

  MemoryDiagnosticTransaction_Create((uintptr_t)dummy_memory,
                                     sizeof(dummy_memory), sizeof(uint32_t));
  MemoryDiagnosticTransaction_Add(0x12345678);
  MemoryDiagnosticTransaction_Add(0);
  bool is_success = MemoryDiagnosticTransaction_Execute();

  EXPECT_TRUE(is_success);
  for (int i = 0; i < 8; ++i)
    ASSERT_EQ(0, dummy_memory[i]) << "Failure at index " << i;
}

TEST_F(MemoryDiagnosticTransactionTest, AddMoreThanBusWidth) {
  uint8_t dummy_memory[8] = {0};

  MemoryDiagnosticTransaction_Create((uintptr_t)dummy_memory,
                                     sizeof(dummy_memory), sizeof(uint8_t));
  MemoryDiagnosticTransaction_Add(0xC3);
  MemoryDiagnosticTransaction_Add(0x1234);
  bool is_success = MemoryDiagnosticTransaction_Execute();

  EXPECT_FALSE(is_success);
  for (int i = 0; i < 8; ++i)
    ASSERT_EQ(0xC3, dummy_memory[i]) << "Failure at index " << i;
}

TEST_F(MemoryDiagnosticTransactionTest, AddAndExecuteBeforeCreate) {
  MemoryDiagnosticTransaction_Add(0x1234);
  bool is_success = MemoryDiagnosticTransaction_Execute();

  EXPECT_FALSE(is_success);
}
