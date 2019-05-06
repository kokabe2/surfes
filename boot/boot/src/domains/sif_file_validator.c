// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "sif_file_validator.h"

#include <stdbool.h>

#include "common/runtime_error.h"
#include "sif_header.h"

static bool IsSifFile(SifHeader header) {
  const uint8_t kMagicNumber[] = {0x7F, 'S', 'I', 'F'};
  for (int i = 0; i < sizeof(kMagicNumber); ++i)
    if (header->identification[i] != kMagicNumber[i]) {
      RUNTINE_ERROR("SIF File Validator: non SIF file", i);
      return false;
    }

  return true;
}

static bool IsValidClass(SifHeader header) {
  if (header->identification[kSifIdClass] == kSc32) return true;
  if (header->identification[kSifIdClass] == kSc64) return true;

  RUNTINE_ERROR("SIF File Validator: invalid SIF class",
                header->identification[kSifIdClass]);
  return false;
}

static bool IsValidVersion(SifHeader header) {
  if (header->identification[kSifIdVersion] == kSvCurrent) return true;

  RUNTINE_ERROR("SIF File Validator: invalid SIF version",
                header->identification[kSifIdVersion]);
  return false;
}

static bool IsValidHeaderSize(SifHeader header) {
  const uint16_t kHeaderSize[] = {kSifHeaderSizeInClass32,
                                  kSifHeaderSizeInClass64};
  uint8_t sif_class = header->identification[kSifIdClass];
  if (header->header_size == kHeaderSize[sif_class - 1]) return true;

  RUNTINE_ERROR("SIF File Validator: invalid header size", header->header_size);
  return false;
}

static bool IsValidFileSize(SifHeader header) {
  if (header->file_size >= header->header_size && !(header->file_size % 4))
    return true;

  RUNTINE_ERROR("SIF File Validator: invalid file size", header->file_size);
  return false;
}

static bool HasFileAddress(SifHeader header) {
  if (header->file_address) return true;

  RUNTINE_ERROR("SIF File Validator: no file address", header->file_address);
  return false;
}

static bool HasNoDataCorruption(SifHeader header) {
  uint32_t sum = 0;
  int loop = header->file_size / sizeof(uint32_t);
  uint32_t* data = (uint32_t*)header;
  for (int i = 0; i < loop; ++i) sum += data[i];
  if (!sum) return true;

  return false;
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
