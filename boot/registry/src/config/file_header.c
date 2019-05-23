// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "immutable_registry.h"
#include "registry_api.h"
#include "sif_header.h"

extern const RegistryApiStruct kRegistyApi;

const SifHeaderStruct kFileHeader = {
    .identification = {0x7F, 'S', 'I', 'F', kSc32, kSd2Lsb, kSvCurrent},
    .type = kStData,
    .machine = kSmRx,
    .header_size = sizeof(SifHeaderStruct),
    .file_version = 0x0000000100000000,
    .file_address = kRegistryFileAddress,
    .entry_point = (uintptr_t)&kRegistyApi,
};
