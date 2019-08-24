// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "system_time.h"

#include "impl/utkernel.h"

void SystemTime_Create(uint64_t current_time) {
  SYSTIM packet = {
      .hi = (W)(current_time >> 32),
      .lo = (UW)(current_time & 0x00000000FFFFFFFF),
  };
  tk_set_tim(&packet);
}

void SystemTime_Destroy(void) { SystemTime_Create(0); }

uint64_t SystemTime_get(void) {
  SYSTIM packet;
  tk_get_tim(&packet);
  return (packet.hi << 32) + packet.lo;
}
