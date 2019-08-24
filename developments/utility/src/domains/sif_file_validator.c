// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "sif_file_validator.h"

#include <stdbool.h>

#include "modular_sum.h"
#include "sif_header.h"

static bool IsSifFile(SifHeader header) {
  uint8_t magic_number[] = {0x7F, 'S', 'I', 'F'};
  for (int i = 0; i < sizeof(magic_number); ++i)
    if (header->identification[i] != magic_number[i]) return false;
  return true;
}

static bool IsValidClass(SifHeader header) {
  if (header->identification[kSiiClass] == kSc32) return true;
  if (header->identification[kSiiClass] == kSc64) return true;
  return false;
}

static bool IsValidVersion(SifHeader header) {
  if (header->identification[kSiiVersion] == kSvCurrent) return true;
  return false;
}

static uint16_t headerSize(uint8_t sif_class) {
  uint16_t header_size[] = {kShdsHeaderSizeInClass32, kShdsHeaderSizeInClass64};

  return header_size[sif_class - 1];
}

static bool IsValidHeaderSize(SifHeader header) {
  uint8_t sif_class = header->identification[kSiiClass];
  if (header->header_size == headerSize(sif_class)) return true;
  return false;
}

static bool IsValidFileSize(SifHeader header) {
  if (header->file_size >= header->header_size) return true;
  return false;
}

static bool HasNoDataCorruption(SifHeader header) {
  if (ModularSum_Verify((uint32_t*)header->file_address, header->file_size))
    return false;

  return true;
}

static const struct {
  bool (*Validate)(SifHeader);
  int error_code;
} kValidators[] = {
    {IsSifFile, kSfveMagicNumberError},
    {IsValidClass, kSfveClassError},
    {IsValidVersion, kSfveVersionError},
    {IsValidHeaderSize, kSfveHeaderSizeError},
    {IsValidFileSize, kSfveFileSizeError},
    {HasNoDataCorruption, kSfveChecksumError},
};
int SifFileValidator_Validate(uintptr_t file_address) {
  SifHeader header = (SifHeader)file_address;
  int num_of_validators = sizeof(kValidators) / sizeof(kValidators[0]);
  for (int i = 0; i < num_of_validators; ++i)
    if (!kValidators[i].Validate(header)) return kValidators[i].error_code;

  return 0;
}
