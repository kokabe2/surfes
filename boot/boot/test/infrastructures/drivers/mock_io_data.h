// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_TEST_INFRASTRUCTURES_DRIVERS_MOCK_IO_DATA_H_
#define BOOT_BOOT_TEST_INFRASTRUCTURES_DRIVERS_MOCK_IO_DATA_H_

extern "C" {
#include "infrastructures/drivers/io_data.h"
}

typedef uint32_t ioData;

void MockIoData_Create(int expectation_count);
void MockIoData_Destroy(void);
void MockIoData_ExpectWrite(ioAddress offset, ioData data);
void MockIoData_ExpectReadThenReturn(ioAddress offset, ioData to_return);
void MockIoData_VerifyCompletion(void);

#endif  // BOOT_BOOT_TEST_INFRASTRUCTURES_DRIVERS_MOCK_IO_DATA_H_
