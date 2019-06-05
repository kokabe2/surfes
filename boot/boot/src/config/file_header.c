// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "sif_header.h"

#include "immutable_registry.h"

const SifHeaderStruct kFileHeader = {
    .identification = {0x7F, 'S', 'I', 'F', kSc32, kSd2Lsb, kSvCurrent},
    .type = kStExe,
    .machine = kSmRx,
    .header_size = sizeof(SifHeaderStruct),
    .file_version = kBootFileVersion,
    .file_address = kBootFileAddress,
    .entry_point = 0xFFFFFFFC,
};
