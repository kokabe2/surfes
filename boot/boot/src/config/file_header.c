// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "sif_header.h"

const SifHeaderStruct kFileHeader = {
    .identification = {0x7F, 'S', 'I', 'F', kSc32, kSd2Lsb, kSvCurrent},
    .type = kStExe,
    .machine = kSmRx,
    .header_size = sizeof(SifHeaderStruct),
    .file_version = 0x0000000100000000,
    .file_size = 8 * 1024,
    .file_address = 0xFFFFE000,
    .entry_point = 0xFFFFFFFC,
};
