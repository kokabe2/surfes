// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "fake_time.h"

#include "utkernel.h"

static SYSTIM system_time;

void fake_time_init(void) {
  system_time.hi = 0;
  system_time.lo = 0;
}

void fake_time_increment(uint64_t time) {
  system_time.hi += (W)(time >> 32);
  system_time.lo += (UW)(time & 0x00000000FFFFFFFF);
}

ER tk_set_tim(CONST SYSTIM *pk_tim) {
  if (!pk_tim) return E_PAR;
  system_time.hi = pk_tim->hi;
  system_time.lo = pk_tim->lo;
  return E_OK;
}

ER tk_get_tim(SYSTIM *pk_tim) {
  if (!pk_tim) return E_PAR;
  pk_tim->hi = system_time.hi;
  pk_tim->lo = system_time.lo;
  return E_OK;
}
