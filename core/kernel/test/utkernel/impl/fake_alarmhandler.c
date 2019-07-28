// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "fake_alarmhandler.h"

#include <stddef.h>

enum {
  kLowestId = 0,
  kHighestId = CFN_MAX_ALMID - 1,
  kNonsenseId = ~0,
};

typedef struct {
  void *exinf;
  ATR almatr;
  FP almhdr;
  RELTIM lfttim;
  UINT almstat;
} AlarmHandlerControlBlockStruct;

static AlarmHandlerControlBlockStruct control_blocks[CFN_MAX_ALMID];

static void InitBlock(ID almid) {
  control_blocks[almid].exinf = NULL;
  control_blocks[almid].almatr = ~0;
  control_blocks[almid].almhdr = NULL;
  control_blocks[almid].lfttim = 0;
  control_blocks[almid].almstat = TALM_STP;
}

void fake_alarmhandler_init(void) {
  for (ID i = kLowestId; i <= kHighestId; ++i) InitBlock(i);
}

void *fake_alarmhandler_getExtendedInformation(ID almid) {
  return control_blocks[almid].exinf;
}

ATR fake_alarmhandler_getAttribute(ID almid) {
  return control_blocks[almid].almatr;
}

FP fake_alarmhandler_getHandler(ID almid) {
  return control_blocks[almid].almhdr;
}

RELTIM fake_alarmhandler_getLeftTime(ID almid) {
  return control_blocks[almid].lfttim;
}

bool fake_alarmhandler_isActive(ID almid) {
  return control_blocks[almid].almstat == TALM_STA;
}

bool fake_alarmhandler_isCreated(ID almid) {
  return control_blocks[almid].almhdr != NULL;
}

void fake_alarmhandler_countdown(ID almid, RELTIM time) {
  if (control_blocks[almid].lfttim <= time) {
    control_blocks[almid].lfttim = 0;
    control_blocks[almid].almhdr(control_blocks[almid].exinf);
    control_blocks[almid].almstat = TALM_STP;
  } else {
    control_blocks[almid].lfttim -= time;
  }
}

static ID RetrieveUnusedId(void) {
  for (ID i = kLowestId; i <= kHighestId; ++i)
    if (!control_blocks[i].almhdr) return i;
  return kNonsenseId;
}

ID tk_cre_alm(CONST T_CALM *pk_calm) {
  if (!pk_calm || !pk_calm->almhdr) return E_PAR;

  ID almid = RetrieveUnusedId();
  if (almid == kNonsenseId) return E_LIMIT;

  control_blocks[almid].exinf = pk_calm->exinf;
  control_blocks[almid].almatr = pk_calm->almatr;
  control_blocks[almid].almhdr = pk_calm->almhdr;

  return almid;
}

ER tk_del_alm(ID almid) {
  if (almid < kLowestId || almid > kHighestId) return E_ID;
  if (!control_blocks[almid].almhdr) return E_NOEXS;

  InitBlock(almid);

  return E_OK;
}

ER tk_sta_alm(ID almid, RELTIM almtim) {
  if (almid < kLowestId || almid > kHighestId) return E_ID;
  if (!control_blocks[almid].almhdr) return E_NOEXS;

  control_blocks[almid].lfttim = almtim;
  control_blocks[almid].almstat = TALM_STA;

  return E_OK;
}

ER tk_stp_alm(ID almid) {
  if (almid < kLowestId || almid > kHighestId) return E_ID;
  if (!control_blocks[almid].almhdr) return E_NOEXS;

  control_blocks[almid].lfttim = 0;
  control_blocks[almid].almstat = TALM_STP;

  return E_OK;
}

ER tk_ref_alm(ID almid, T_RALM *pk_ralm) {
  if (almid < kLowestId || almid > kHighestId) return E_ID;
  if (!control_blocks[almid].almhdr) return E_NOEXS;
  if (!pk_ralm) return E_PAR;

  pk_ralm->exinf = control_blocks[almid].exinf;
  pk_ralm->lfttim = control_blocks[almid].lfttim;
  pk_ralm->almstat = control_blocks[almid].almstat;

  return E_OK;
}
