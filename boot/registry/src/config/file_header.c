// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "registries/int_registry_getter.h"
#include "registries/uintptr_registry_getter.h"
#include "registry_file_api.h"
#include "registry_fixed_api.h"
#include "sif_header.h"

const RegistryFileEntryPointStruct kEntryPoint = {
    .getIntRegistryValue = IntRegistryGetter_get,
    .getUintptrRegistryValue = UintptrRegistryGetter_get,
};

const SifHeaderStruct kFileHeader = {
    .identification = {0x7F, 'S', 'I', 'F', kSc32, kSd2Lsb, kSvCurrent},
    .type = kStData,
    .machine = kSmRx,
    .header_size = sizeof(SifHeaderStruct),
    .file_version = 0x0000000100000000,
    .file_address = kRegistryFileAddress,
    .entry_point = (uintptr_t)&kEntryPoint,
};
