// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "fake_task.h"

#include <stddef.h>

#include "utkernel.h"

enum {
  kReservedId = 0,
  kLowestId,
  kHighestId = CFN_MAX_TSKID - 1,
  kNonsenseId = ~0,
};

typedef struct {
  void *exinf;
  ATR tskatr;
  FP task;
  PRI itskpri;
  SZ stksz;
  INT stacd;
  UINT state;
  TMO tmout;
  INT suscnt;
  INT wupcnt;
} TaskControlBlockStruct;

static TaskControlBlockStruct task_control_blocks[CFN_MAX_TSKID];

static void InitBlock(ID tskid) {
  task_control_blocks[tskid].exinf = NULL;
  task_control_blocks[tskid].tskatr = ~0;
  task_control_blocks[tskid].task = NULL;
  task_control_blocks[tskid].itskpri = ~0;
  task_control_blocks[tskid].stksz = 0;
  task_control_blocks[tskid].stacd = 0;
  task_control_blocks[tskid].state = TTS_NONE;
  task_control_blocks[tskid].tmout = 0;
  task_control_blocks[tskid].suscnt = 0;
  task_control_blocks[tskid].wupcnt = 0;
}

void fake_task_init(void) {
  for (ID i = 0; i < CFN_MAX_TSKID; ++i) InitBlock(i);
}

ATR fake_task_getAttribute(ID tskid) {
  return task_control_blocks[tskid].tskatr;
}

PRI fake_task_getPriority(ID tskid) {
  return task_control_blocks[tskid].itskpri;
}

SZ fake_task_getStackSize(ID tskid) { return task_control_blocks[tskid].stksz; }

UINT fake_task_getState(ID tskid) { return task_control_blocks[tskid].state; }

void fake_task_setState(ID tskid, UINT state) {
  task_control_blocks[tskid].state = state;
}

TMO fake_task_getTimeout(ID tskid) { return task_control_blocks[tskid].tmout; }

INT fake_task_getSuspendCount(ID tskid) {
  return task_control_blocks[tskid].suscnt;
}

INT fake_task_getWakeupCount(ID tskid) {
  return task_control_blocks[tskid].wupcnt;
}

bool fake_task_dispatch(ID tskid) {
  if (task_control_blocks[tskid].state != TTS_RDY) return false;

  task_control_blocks[tskid].state = TTS_RUN;
  task_control_blocks[tskid].task(task_control_blocks[tskid].stacd,
                                  task_control_blocks[tskid].exinf);
  return true;
}

static ID RetrieveTaskId(UINT state) {
  for (ID i = kLowestId; i <= kHighestId; ++i)
    if (task_control_blocks[i].state == state) return i;
  return kNonsenseId;
}

ID tk_cre_tsk(CONST T_CTSK *pk_ctsk) {
  if ((pk_ctsk == NULL) || (pk_ctsk->itskpri < 1) ||
      (pk_ctsk->itskpri > TK_MAX_TSKPRI) || (pk_ctsk->stksz <= 0))
    return E_PAR;

  ID tskid = RetrieveTaskId(TTS_NONE);
  if (tskid == kNonsenseId) return E_LIMIT;

  task_control_blocks[tskid].exinf = pk_ctsk->exinf;
  task_control_blocks[tskid].tskatr = pk_ctsk->tskatr;
  task_control_blocks[tskid].task = pk_ctsk->task;
  task_control_blocks[tskid].itskpri = pk_ctsk->itskpri;
  task_control_blocks[tskid].stksz = pk_ctsk->stksz;
  task_control_blocks[tskid].state = TTS_DMT;

  return tskid;
}

ER tk_del_tsk(ID tskid) {
  if (tskid < kLowestId || tskid > kHighestId) return E_ID;
  if (task_control_blocks[tskid].state == TTS_NONE) return E_NOEXS;
  if (task_control_blocks[tskid].state != TTS_DMT) return E_OBJ;

  InitBlock(tskid);
  return E_OK;
}

ER tk_sta_tsk(ID tskid, INT stacd) {
  if (tskid < kLowestId || tskid > kHighestId) return E_ID;
  if (task_control_blocks[tskid].state == TTS_NONE) return E_NOEXS;
  if (task_control_blocks[tskid].state != TTS_DMT) return E_OBJ;

  task_control_blocks[tskid].stacd = stacd;
  task_control_blocks[tskid].state = TTS_RDY;
  return E_OK;
}

void tk_exd_tsk(void) {
  ID tskid = RetrieveTaskId(TTS_RUN);
  if (tskid != kNonsenseId) InitBlock(tskid);
}

ER tk_ter_tsk(ID tskid) {
  if (tskid < kLowestId || tskid > kHighestId) return E_ID;
  if (task_control_blocks[tskid].state == TTS_NONE) return E_NOEXS;
  if ((task_control_blocks[tskid].state == TTS_DMT) ||
      (task_control_blocks[tskid].state == TTS_RUN))
    return E_OBJ;

  task_control_blocks[tskid].state = TTS_DMT;
  return E_OK;
}

ER tk_slp_tsk(TMO tmout) {
  ID tskid = RetrieveTaskId(TTS_RUN);
  if (tskid == kNonsenseId) return E_CTX;

  if (task_control_blocks[tskid].wupcnt) task_control_blocks[tskid].wupcnt--;
  if (task_control_blocks[tskid].wupcnt == 0) {
    task_control_blocks[tskid].state = TTS_WAI;
    task_control_blocks[tskid].tmout = tmout;
  }
  return E_OK;
}

ER tk_wup_tsk(ID tskid) {
  if (tskid < kLowestId || tskid > kHighestId) return E_ID;
  if (task_control_blocks[tskid].state == TTS_NONE) return E_NOEXS;
  if ((task_control_blocks[tskid].state == TTS_DMT) ||
      (task_control_blocks[tskid].state == TTS_RUN))
    return E_OBJ;

  if (task_control_blocks[tskid].state & TTS_WAI) {
    task_control_blocks[tskid].state =
        task_control_blocks[tskid].state == TTS_WAI
            ? TTS_RDY
            : (task_control_blocks[tskid].state & ~TTS_WAI);
    task_control_blocks[tskid].tmout = 0;
  } else {
    task_control_blocks[tskid].wupcnt++;
  }
  return E_OK;
}

ER tk_rel_wai(ID tskid) {
  if (tskid < kLowestId || tskid > kHighestId) return E_ID;
  if (task_control_blocks[tskid].state == TTS_NONE) return E_NOEXS;
  if (!(task_control_blocks[tskid].state & TTS_WAI)) return E_OBJ;

  task_control_blocks[tskid].state =
      task_control_blocks[tskid].state == TTS_WAI
          ? TTS_RDY
          : (task_control_blocks[tskid].state & ~TTS_WAI);
  task_control_blocks[tskid].tmout = 0;
  return E_OK;
}

ER tk_sus_tsk(ID tskid) {
  if (tskid < kLowestId || tskid > kHighestId) return E_ID;
  if (task_control_blocks[tskid].state == TTS_NONE) return E_NOEXS;
  if ((task_control_blocks[tskid].state == TTS_DMT) ||
      (task_control_blocks[tskid].state == TTS_RUN))
    return E_OBJ;

  task_control_blocks[tskid].state =
      task_control_blocks[tskid].state == TTS_RDY
          ? TTS_SUS
          : (task_control_blocks[tskid].state | TTS_SUS);
  task_control_blocks[tskid].suscnt++;
  return E_OK;
}

ER tk_rsm_tsk(ID tskid) {
  if (tskid < kLowestId || tskid > kHighestId) return E_ID;
  if (task_control_blocks[tskid].state == TTS_NONE) return E_NOEXS;
  if (!(task_control_blocks[tskid].state & TTS_SUS)) return E_OBJ;

  task_control_blocks[tskid].suscnt--;
  if (task_control_blocks[tskid].suscnt == 0)
    task_control_blocks[tskid].state =
        task_control_blocks[tskid].state == TTS_SUS
            ? TTS_RDY
            : (task_control_blocks[tskid].state & ~TTS_SUS);
  return E_OK;
}

ER tk_dly_tsk(RELTIM dlytim) {
  ID tskid = RetrieveTaskId(TTS_RUN);
  if (tskid == kNonsenseId) return E_CTX;

  task_control_blocks[tskid].state = TTS_WAI;
  task_control_blocks[tskid].tmout = (TMO)dlytim;
  return E_OK;
}

ID tk_get_tid(void) {
  ID tskid = RetrieveTaskId(TTS_RUN);
  if (tskid == kNonsenseId) tskid = 0;
  return tskid;
}
