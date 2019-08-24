// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "system_file_entry_point.h"

int BootStrapper(int runlevel) { return 0; }

const SystemFileEntryPointStruct kSystemFileEntryPoint = {
    .Execute = BootStrapper,
};
