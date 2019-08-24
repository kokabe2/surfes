// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "sif_file_validator.h"

#include <stdbool.h>
#include <string.h>

#include "modular_sum.h"
#include "sif_header.h"

static bool IsSifFile(SifHeader header) {
  const uint8_t kMagicNumber[] = {0x7F, 'S', 'I', 'F'};
  return memcmp(header->identification, kMagicNumber, sizeof(kMagicNumber)) ==
         0;
}

static bool IsValidClass(SifHeader header) {
  return (header->identification[kSiiClass] == kSc32) ||
         (header->identification[kSiiClass] == kSc64);
}

static bool IsValidVersion(SifHeader header) {
  return header->identification[kSiiVersion] == kSvCurrent;
}

static uint16_t headerSizeOf(uint8_t sif_class) {
  const uint16_t kHeaderSize[] = {kShdsHeaderSizeInClass32,
                                  kShdsHeaderSizeInClass64};
  return kHeaderSize[sif_class - 1];
}

static bool IsValidHeaderSize(SifHeader header) {
  uint8_t sif_class = header->identification[kSiiClass];
  return header->header_size == headerSizeOf(sif_class);
}

static bool IsValidFileSize(SifHeader header) {
  return header->file_size >= header->header_size;
}

static bool HasNoDataCorruption(SifHeader header) {
  return ModularSum_Verify((uint32_t*)header->file_address,
                           header->file_size) == 0;
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
