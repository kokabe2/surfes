// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_TEST_UTKERNEL_IMPL_UTKERNEL_H_
#define CORE_KERNEL_TEST_UTKERNEL_IMPL_UTKERNEL_H_

#define CONST const

#define TA_HLNG 0x00000001
#define TA_RNG0 0x00000000

#define E_OK (0)
#define E_PAR (-17)
#define E_ID (-18)
#define E_CTX (-25)
#define E_LIMIT (-34)
#define E_OBJ (-41)
#define E_NOEXS (-42)

#define TMO_FEVR (-1)

#define CFN_MAX_TSKID (32)
#define TK_MAX_TSKPRI (16)

typedef signed long W;
typedef unsigned char UB;
typedef unsigned long UW;
typedef signed int INT;
typedef unsigned int UINT;
typedef UW ATR;
typedef INT PRI;
typedef W SZ;
typedef INT ID;
typedef INT ER;
typedef W TMO;
typedef void (*FP)();

typedef struct {
  void *exinf;
  ATR tskatr;
  FP task;
  PRI itskpri;
  SZ stksz;
  SZ sstksz;
  void *stkptr;
  void *uatb;
  INT lsid;
  ID resid;
  UB dsname[8];
  void *bufptr;
} T_CTSK;
typedef struct {
  ATR intatr;
  FP inthdr;
} T_DINT;

ID tk_cre_tsk(CONST T_CTSK *pk_ctsk);
ER tk_del_tsk(ID tskid);
ER tk_sta_tsk(ID tskid, INT stacd);
void tk_exd_tsk(void);
ER tk_ter_tsk(ID tskid);

ER tk_slp_tsk(TMO tmout);
ER tk_wup_tsk(ID tskid);
ER tk_rel_wai(ID tskid);
ER tk_sus_tsk(ID tskid);
ER tk_rsm_tsk(ID tskid);

ID tk_get_tid(void);

ER tk_def_int(UINT intno, CONST T_DINT *pk_dint);
void EnableInt(UINT intno, INT level);
void DisableInt(UINT intno);

#endif  // CORE_KERNEL_TEST_UTKERNEL_IMPL_UTKERNEL_H_
