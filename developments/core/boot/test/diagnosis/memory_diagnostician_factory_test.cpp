// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "diagnosis/memory_diagnostician_factory.h"
}

TEST(MemoryDiagnosticianFactoryTest, DiagnosticMemoryIn8Bit) {
  uint8_t dummy_memory[1024 + 1] = {0};
  IMemoryDiagnosable imd = MemoryDiagnosticianFactory_Make(sizeof(uint8_t));
  ASSERT_TRUE(imd != NULL);

  EXPECT_TRUE(imd->ReadAfterWrite((uintptr_t)dummy_memory, 1024, 0x5A));
  for (int i = 0; i < 1024; ++i)
    ASSERT_EQ(0x5A, dummy_memory[i]) << "Failure at index " << i;
  EXPECT_EQ(0, dummy_memory[1024]);
}

TEST(MemoryDiagnosticianFactoryTest, DiagnosticMemoryIn16Bit) {
  uint16_t dummy_memory[1024 + 1] = {0};
  IMemoryDiagnosable imd = MemoryDiagnosticianFactory_Make(sizeof(uint16_t));
  ASSERT_TRUE(imd != NULL);

  EXPECT_TRUE(imd->ReadAfterWrite((uintptr_t)dummy_memory, 2048, 0x5AA5));
  for (int i = 0; i < 1024; ++i)
    ASSERT_EQ(0x5AA5, dummy_memory[i]) << "Failure at index " << i;
  EXPECT_EQ(0, dummy_memory[1024]);
}

TEST(MemoryDiagnosticianFactoryTest, DiagnosticMemoryIn32Bit) {
  uint32_t dummy_memory[1024 + 1] = {0};
  IMemoryDiagnosable imd = MemoryDiagnosticianFactory_Make(sizeof(uint32_t));
  ASSERT_TRUE(imd != NULL);

  EXPECT_TRUE(imd->ReadAfterWrite((uintptr_t)dummy_memory, 4096, 0x5AA59669));
  for (int i = 0; i < 1024; ++i)
    ASSERT_EQ(0x5AA59669, dummy_memory[i]) << "Failure at index " << i;
  EXPECT_EQ(0, dummy_memory[1024]);
}

TEST(MemoryDiagnosticianFactoryTest, MakeWithInvalidBusWidth) {
  IMemoryDiagnosable imd = MemoryDiagnosticianFactory_Make(0);
  ASSERT_TRUE(imd != NULL);
  EXPECT_EQ(imd, MemoryDiagnosticianFactory_Make(3));
  EXPECT_EQ(imd, MemoryDiagnosticianFactory_Make(-1));
  EXPECT_EQ(imd, MemoryDiagnosticianFactory_Make(1314));

  uint8_t dummy_memory[1] = {0};
  EXPECT_FALSE(imd->ReadAfterWrite((uintptr_t)dummy_memory, 1, 0x5A));
}

TEST(MemoryDiagnosticianFactoryTest, ReadAfterWriteWithSizeLessThanZero) {
  uint8_t dummy_memory[1] = {0};
  IMemoryDiagnosable imd = MemoryDiagnosticianFactory_Make(sizeof(uint8_t));

  EXPECT_FALSE(imd->ReadAfterWrite((uintptr_t)dummy_memory, -16, 0x5A));
  EXPECT_EQ(0, dummy_memory[0]);
}

TEST(MemoryDiagnosticianFactoryTest, ReadAfterWriteWithMemoryOverflow) {
  IMemoryDiagnosable imd = MemoryDiagnosticianFactory_Make(sizeof(uint8_t));

  EXPECT_FALSE(imd->ReadAfterWrite(UINTPTR_MAX, 1, 0x5A));
}

TEST(MemoryDiagnosticianFactoryTest, ReadAfterWriteWithOverSizeBitPattern) {
  uint8_t dummy_memory[1] = {0};
  IMemoryDiagnosable imd = MemoryDiagnosticianFactory_Make(sizeof(uint8_t));

  EXPECT_FALSE(imd->ReadAfterWrite((uintptr_t)dummy_memory, 1, 0x5AA5));
  EXPECT_EQ(0, dummy_memory[0]);
}
