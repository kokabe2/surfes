// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_DOMAINS_SIF_FILE_VALIDATOR_H_
#define BOOT_BOOT_SRC_DOMAINS_SIF_FILE_VALIDATOR_H_

#include <stdint.h>

enum SifFileValidatorError {
  kSfvNoError = 0,
  kSfvMagicNumberError,
  kSfvClassError,
  kSfvVersionError,
  kSfvHeaderSizeError,
  kSfvFileSizeError,
  kSfvFileAddressError,
  kSfvChecksumError,
};

int SifFileValidator_Validate(uintptr_t file_address);

#endif  // BOOT_BOOT_SRC_DOMAINS_SIF_FILE_VALIDATOR_H_
