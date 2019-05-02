// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_TEST_COMMON_SPY_RUNTIME_ERROR_H_
#define BOOT_BOOT_TEST_COMMON_SPY_RUNTIME_ERROR_H_

void SpyRuntimeError_Reset(void);
const char* SpyRuntimeError_GetLastError(void);
int SpyRuntimeError_GetLastParameter(void);

#endif  // BOOT_BOOT_TEST_COMMON_SPY_RUNTIME_ERROR_H_
