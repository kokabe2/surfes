// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "fake_cyclichandler.h"

#include <stddef.h>

enum {
  kLowestId = 0,
  kHighestId = CFN_MAX_CYCID - 1,
  kNonsenseId = ~0,
};

typedef struct {
  void *exinf;
  ATR cycatr;
  FP cychdr;
  RELTIM lfttim;
  RELTIM cyctim;
  UINT cycstat;
} AlarmHandlerControlBlockStruct;

static AlarmHandlerControlBlockStruct control_blocks[CFN_MAX_CYCID];

static void InitBlock(ID cycid) {
  control_blocks[cycid].exinf = NULL;
  control_blocks[cycid].cycatr = ~0;
  control_blocks[cycid].cychdr = NULL;
  control_blocks[cycid].lfttim = 0;
  control_blocks[cycid].cyctim = 0;
  control_blocks[cycid].cycstat = TCYC_STP;
}

void fake_cyclichandler_init(void) {
  for (ID i = kLowestId; i <= kHighestId; ++i) InitBlock(i);
}

ATR fake_cyclichandler_getAttribute(ID cycid) {
  return control_blocks[cycid].cycatr;
}

RELTIM fake_cyclichandler_getLeftTime(ID cycid) {
  return control_blocks[cycid].lfttim;
}

RELTIM fake_cyclichandler_getCyclicTime(ID cycid) {
  return control_blocks[cycid].cyctim;
}

bool fake_cyclichandler_isActive(ID cycid) {
  return control_blocks[cycid].cycstat == TCYC_STA;
}

bool fake_cyclichandler_isCreated(ID cycid) {
  return control_blocks[cycid].cychdr != NULL;
}

void fake_cyclichandler_countdown(ID cycid, RELTIM time) {
  if (control_blocks[cycid].cycstat != TCYC_STA) return;
  if (control_blocks[cycid].lfttim <= time) {
    for (; time >= control_blocks[cycid].cyctim;
         time -= control_blocks[cycid].cyctim)
      control_blocks[cycid].cychdr(control_blocks[cycid].exinf);
    control_blocks[cycid].lfttim = control_blocks[cycid].cyctim - time;
  } else {
    control_blocks[cycid].lfttim -= time;
  }
}

static ID RetrieveUnusedId(void) {
  for (ID i = kLowestId; i <= kHighestId; ++i)
    if (!control_blocks[i].cychdr) return i;
  return kNonsenseId;
}

ID tk_cre_cyc(CONST T_CCYC *pk_ccyc) {
  if (!pk_ccyc || !pk_ccyc->cychdr) return E_PAR;

  ID cycid = RetrieveUnusedId();
  if (cycid == kNonsenseId) return E_LIMIT;

  control_blocks[cycid].exinf = pk_ccyc->exinf;
  control_blocks[cycid].cycatr = pk_ccyc->cycatr;
  control_blocks[cycid].cychdr = pk_ccyc->cychdr;
  control_blocks[cycid].cyctim = pk_ccyc->cyctim;
  control_blocks[cycid].lfttim = pk_ccyc->cycphs;
  if (pk_ccyc->cycatr & TA_STA) control_blocks[cycid].cycstat = TCYC_STA;

  return cycid;
}

ER tk_del_cyc(ID cycid) {
  if (cycid < kLowestId || cycid > kHighestId) return E_ID;
  if (!control_blocks[cycid].cychdr) return E_NOEXS;

  InitBlock(cycid);

  return E_OK;
}

ER tk_sta_cyc(ID cycid) {
  if (cycid < kLowestId || cycid > kHighestId) return E_ID;
  if (!control_blocks[cycid].cychdr) return E_NOEXS;

  control_blocks[cycid].cycstat = TCYC_STA;
  if (!(control_blocks[cycid].cycatr & TA_PHS))
    control_blocks[cycid].lfttim = control_blocks[cycid].cyctim;

  return E_OK;
}

ER tk_stp_cyc(ID cycid) {
  if (cycid < kLowestId || cycid > kHighestId) return E_ID;
  if (!control_blocks[cycid].cychdr) return E_NOEXS;

  control_blocks[cycid].cycstat = TCYC_STP;

  return E_OK;
}

ER tk_ref_cyc(ID cycid, T_RCYC *pk_rcyc) {
  if (cycid < kLowestId || cycid > kHighestId) return E_ID;
  if (!control_blocks[cycid].cychdr) return E_NOEXS;
  if (!pk_rcyc) return E_PAR;

  pk_rcyc->exinf = control_blocks[cycid].exinf;
  pk_rcyc->lfttim = control_blocks[cycid].lfttim;
  pk_rcyc->cycstat = control_blocks[cycid].cycstat;

  return E_OK;
}
