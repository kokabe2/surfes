// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "sif_header.h"

const SifHeaderStruct kBootFileHeader = {
    {0x7F, 'S', 'I', 'F', kSc32, kSd2Lsb, kSvCurrent},
    kStExe,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    0,
    0,
    0,
    0x0000000100000000,
    8 * 1024,
    0,
    0xFFFFE000,
    0xFFFFFFFC,
    0,
    0,
    "",
};
