// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_INFRASTRUCTURES_FRAMEWORKS_SYSTEM_RUNNER_H_
#define BOOT_BOOT_SRC_INFRASTRUCTURES_FRAMEWORKS_SYSTEM_RUNNER_H_

void SystemRunner_Create(int defalut_runlevel);
void SystemRunner_Destroy(void);
void SystemRunner_Run(void);

#endif  // BOOT_BOOT_SRC_INFRASTRUCTURES_FRAMEWORKS_SYSTEM_RUNNER_H_
