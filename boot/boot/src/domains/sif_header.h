// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_DOMAINS_SIF_HEADER_H_
#define BOOT_BOOT_SRC_DOMAINS_SIF_HEADER_H_

#include <stdint.h>

enum SifIdentification {
  kSifIdMagicNumber0 = 0,
  kSifIdMagicNumber1,
  kSifIdMagicNumber2,
  kSifIdMagicNumber3,
  kSifIdClass,
  kSifIdData,
  kSifIdVersion,
  kSifIdPadding,
};

enum SifClass {
  kScNone = 0,
  kSc32,
  kSc64,
};

enum SifData {
  kSdNone = 0,
  kSd2Lsb,
  kSd2Msb,
};

enum SifVersion {
  kSvNone = 0,
  kSvCurrent,
};

enum SifType {
  kStNone = 0,
  kStData,
  kStExe,
  kStLib,
};

enum SifMachine {
  kSmNone = 0,
  kSmRx = 0x11,
};

enum SifSize {
  kSifIdentificationSize = 16,
  kSifDscriptionSize = 128,
  kSifHeaderSizeInClass32 = 192,
  kSifHeaderSizeInClass64 = 208,
};

typedef struct {
  uint8_t identification[kSifIdentificationSize];
  uint16_t type;
  uint16_t machine;
  uint32_t flags;
  uint16_t header_size;
  uint16_t primary_api_num;
  uint16_t extended_api_num;
  uint16_t reserved;
  uint64_t file_version;
  uint32_t file_size;
  uint32_t checksum;
  uintptr_t file_address;
  uintptr_t entry_point;
  uintptr_t primary_api_address;
  uintptr_t extended_api_address;
  uint8_t description[kSifDscriptionSize];
} SifHeaderStruct, *SifHeader;

#endif  // BOOT_BOOT_SRC_DOMAINS_SIF_HEADER_H_
