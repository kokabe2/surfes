// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "micro_time.h"

static uint64_t micro_time;

void MicroTime_Create(void) { micro_time = 0; }

uint64_t MicroTime_Get(void) { return micro_time; }

void MicroTime_Increase(uint64_t time) { micro_time += time; }
