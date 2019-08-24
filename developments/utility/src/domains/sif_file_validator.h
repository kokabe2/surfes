﻿// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_UTILITY_SRC_DOMAINS_SIF_FILE_VALIDATOR_H_
#define DEVELOPMENTS_UTILITY_SRC_DOMAINS_SIF_FILE_VALIDATOR_H_

#include <stdint.h>

enum SifFileValidationError {
  kSfvNoError = 0,
  kSfvMagicNumberError,
  kSfvClassError,
  kSfvVersionError,
  kSfvHeaderSizeError,
  kSfvFileSizeError,
  kSfvChecksumError,
};

int SifFileValidator_Validate(uintptr_t file_address);

#endif  // DEVELOPMENTS_UTILITY_SRC_DOMAINS_SIF_FILE_VALIDATOR_H_
