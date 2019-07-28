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
  bool active;
} AlarmHandlerControlBlockStruct;

static AlarmHandlerControlBlockStruct control_blocks[CFN_MAX_ALMID];

static void InitBlock(ID almid) {
  control_blocks[almid].exinf = NULL;
  control_blocks[almid].almatr = ~0;
  control_blocks[almid].almhdr = NULL;
  control_blocks[almid].lfttim = 0;
  control_blocks[almid].active = false;
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
  return control_blocks[almid].active;
}

bool fake_alarmhandler_isCreated(ID almid) {
  return control_blocks[almid].almhdr != NULL;
}

void fake_alarmhandler_countdown(ID almid, RELTIM time) {
  if (control_blocks[almid].lfttim <= time) {
    control_blocks[almid].lfttim = 0;
    control_blocks[almid].almhdr(control_blocks[almid].exinf);
    control_blocks[almid].active = false;
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

ER tk_sta_alm(ID almid, RELTIM lfttim) {
  if (almid < kLowestId || almid > kHighestId) return E_ID;
  if (!control_blocks[almid].almhdr) return E_NOEXS;

  control_blocks[almid].lfttim = lfttim;
  control_blocks[almid].active = true;

  return E_OK;
}

ER tk_stp_alm(ID almid) {
  if (almid < kLowestId || almid > kHighestId) return E_ID;
  if (!control_blocks[almid].almhdr) return E_NOEXS;

  control_blocks[almid].lfttim = 0;
  control_blocks[almid].active = false;

  return E_OK;
}
