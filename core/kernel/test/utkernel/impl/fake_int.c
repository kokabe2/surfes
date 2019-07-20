// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "fake_int.h"

#include "utkernel.h"

#define N_INTNO 256

typedef struct {
  ATR intatr;
  FP inthdr;
  INT level;
} UtkIntStruct;

static UtkIntStruct int_control_blocks[N_INTNO];

static void DummyIsr(UINT intno) {}

void fake_int_init(void) {
  for (int i = 0; i < N_INTNO; ++i) {
    int_control_blocks[i].intatr = ~0;
    int_control_blocks[i].inthdr = (FP)DummyIsr;
    int_control_blocks[i].level = -1;
  }
}

FP fake_int_getIsrFunction(UINT intno) {
  return int_control_blocks[intno].inthdr;
}

ATR fake_int_getAttribute(UINT intno) {
  return int_control_blocks[intno].intatr;
}

INT fake_int_getLevel(UINT intno) { return int_control_blocks[intno].level; }

void fake_int_run(UINT intno) {
  if (int_control_blocks[intno].level >= 0)
    int_control_blocks[intno].inthdr(intno);
}

ER tk_def_int(UINT intno, CONST T_DINT *pk_dint) {
  if (intno >= N_INTNO) return E_PAR;
  int_control_blocks[intno].intatr = pk_dint->intatr;
  int_control_blocks[intno].inthdr =
      pk_dint->inthdr ? pk_dint->inthdr : (FP)DummyIsr;
  return E_OK;
}

void EnableInt(UINT intno, INT level) {
  int_control_blocks[intno].level = level;
}

void DisableInt(UINT intno) { int_control_blocks[intno].level = -1; }
