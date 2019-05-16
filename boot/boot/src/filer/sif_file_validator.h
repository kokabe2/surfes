// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_FILER_SIF_FILE_VALIDATOR_H_
#define BOOT_BOOT_SRC_FILER_SIF_FILE_VALIDATOR_H_

#include <stdint.h>

enum SifFileValidationError {
  kSfvNoError = 0,
  kSfvChecksumError,
  kSfvMagicNumberError,
  kSfvClassError,
  kSfvVersionError,
  kSfvHeaderSizeError,
  kSfvFileSizeError,
};

int SifFileValidator_Validate(uintptr_t file_address);

#endif  // BOOT_BOOT_SRC_FILER_SIF_FILE_VALIDATOR_H_
