// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_TEST_UTKERNEL_IMPL_UTKERNEL_H_
#define CORE_KERNEL_TEST_UTKERNEL_IMPL_UTKERNEL_H_

#define CONST const
#define TA_HLNG 0x00000001

#define E_OK (0)
#define E_PAR (-17)

typedef unsigned long UW;
typedef signed int INT;
typedef unsigned int UINT;
typedef UW ATR;
typedef INT ER;
typedef void (*FP)();

typedef struct t_dint {
  ATR intatr;
  FP inthdr;
} T_DINT;

ER tk_def_int(UINT intno, CONST T_DINT *pk_dint);
void EnableInt(UINT intno, INT level);
void DisableInt(UINT intno);

#endif  // CORE_KERNEL_TEST_UTKERNEL_IMPL_UTKERNEL_H_
