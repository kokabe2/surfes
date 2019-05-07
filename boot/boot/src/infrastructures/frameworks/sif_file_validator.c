// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "sif_file_validator.h"

#include <stdbool.h>

#include "common/modular_sum.h"
#include "common/runtime_error.h"
#include "sif_header.h"

static bool IsSifFile(SifHeader header) {
  uint8_t magic_number[] = {0x7F, 'S', 'I', 'F'};
  for (int i = 0; i < sizeof(magic_number); ++i)
    if (header->identification[i] != magic_number[i]) {
      RUNTIME_ERROR("SIF File Validator: non SIF file", i);
      return false;
    }

  return true;
}

static bool IsValidClass(SifHeader header) {
  if (header->identification[kSifIdClass] == kSc32) return true;
  if (header->identification[kSifIdClass] == kSc64) return true;

  RUNTIME_ERROR("SIF File Validator: invalid SIF class",
                header->identification[kSifIdClass]);
  return false;
}

static bool IsValidVersion(SifHeader header) {
  if (header->identification[kSifIdVersion] == kSvCurrent) return true;

  RUNTIME_ERROR("SIF File Validator: invalid SIF version",
                header->identification[kSifIdVersion]);
  return false;
}

static uint16_t headerSize(uint8_t sif_class) {
  uint16_t header_size[] = {kSifHeaderSizeInClass32, kSifHeaderSizeInClass64};

  return header_size[sif_class - 1];
}

static bool IsValidHeaderSize(SifHeader header) {
  uint8_t sif_class = header->identification[kSifIdClass];
  if (header->header_size == headerSize(sif_class)) return true;

  RUNTIME_ERROR("SIF File Validator: invalid header size", header->header_size);
  return false;
}

static bool IsMultiplesOfFour(uint32_t size) { return (size % 4) == 0; }

static bool IsValidFileSize(SifHeader header) {
  if (header->file_size >= header->header_size &&
      IsMultiplesOfFour(header->file_size))
    return true;

  RUNTIME_ERROR("SIF File Validator: invalid file size", header->file_size);
  return false;
}

static bool HasFileAddress(SifHeader header) {
  if (header->file_address) return true;

  RUNTIME_ERROR("SIF File Validator: no file address", header->file_address);
  return false;
}

static bool HasNoDataCorruption(SifHeader header) {
  if (ModularSum_Verify((uint32_t*)header->file_address, header->file_size))
    return false;

  return true;
}

typedef bool (*validator)(SifHeader);
static const validator kValidators[] = {
    IsSifFile,       IsValidClass,   IsValidVersion,      IsValidHeaderSize,
    IsValidFileSize, HasFileAddress, HasNoDataCorruption,
};
int SifFileValidator_Validate(uintptr_t file_address) {
  SifHeader header = (SifHeader)file_address;
  int num_of_validators = sizeof(kValidators) / sizeof(kValidators[0]);
  for (int i = 0; i < num_of_validators; ++i)
    if (!kValidators[i](header)) return i + 1;

  return 0;
}
