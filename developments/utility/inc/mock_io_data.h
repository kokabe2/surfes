// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_UTILITY_INC_MOCK_IO_DATA_H_
#define DEVELOPMENTS_UTILITY_INC_MOCK_IO_DATA_H_

extern "C" {
#include "io_data.h"
}

typedef uint32_t ioData;

void MockIoData_Create(int expectation_count);
void MockIoData_Destroy(void);
void MockIoData_ExpectWrite(ioAddress offset, ioData data);
void MockIoData_ExpectReadThenReturn(ioAddress offset, ioData to_return);
void MockIoData_VerifyCompletion(void);

#endif  // DEVELOPMENTS_UTILITY_INC_MOCK_IO_DATA_H_
