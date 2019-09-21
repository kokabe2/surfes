// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_UTILITY_SRC_INFRASTRUCTURES_FRAMEWORKS_SIF_FILE_VALIDATOR_H_
#define DEVELOPMENTS_UTILITY_SRC_INFRASTRUCTURES_FRAMEWORKS_SIF_FILE_VALIDATOR_H_

#include <stdint.h>

enum SifFileValidationError {
  kSfveNoError = 0,
  kSfveMagicNumberError,
  kSfveClassError,
  kSfveVersionError,
  kSfveHeaderSizeError,
  kSfveFileSizeError,
  kSfveChecksumError,
};

int SifFileValidator_Validate(uintptr_t file_address);

#endif  // DEVELOPMENTS_UTILITY_SRC_INFRASTRUCTURES_FRAMEWORKS_SIF_FILE_VALIDATOR_H_
