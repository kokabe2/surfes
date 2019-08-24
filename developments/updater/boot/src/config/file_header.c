// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "immutable_registry.h"
#include "sif_header.h"
#include "system_file_entry_point.h"

extern const SystemFileEntryPointStruct kSystemFileEntryPoint;

const SifHeaderStruct kFileHeader = {
    .identification = {0x7F, 'S', 'I', 'F', kSc32, kSd2Lsb, kSvCurrent},
    .type = kStExe,
    .machine = kSmRx,
    .header_size = sizeof(SifHeaderStruct),
    .file_version = 0x0000000100000000,
    .file_address = (uintptr_t)kRecoveryFileAddress,
    .entry_point = (uintptr_t)&kSystemFileEntryPoint,
};
