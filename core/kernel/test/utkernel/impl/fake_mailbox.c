// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "fake_mailbox.h"

#include <stddef.h>

enum {
  kLowestId = 0,
  kHighestId = CFN_MAX_MBXID - 1,
  kNonsenseId = ~0,
};

typedef struct {
  bool used;
  ATR mbxatr;
  void* last_mail;
} MailboxControlBlockStruct;

static MailboxControlBlockStruct mailbox_control_blocks[CFN_MAX_MBXID];

static void InitBlock(ID mbxid) {
  mailbox_control_blocks[mbxid].used = false;
  mailbox_control_blocks[mbxid].mbxatr = ~0;
  mailbox_control_blocks[mbxid].last_mail = NULL;
}

void fake_mailbox_init(void) {
  for (ID i = kLowestId; i <= kHighestId; ++i) InitBlock(i);
}

ATR fake_mailbox_getAttribute(ID mbxid) {
  return mailbox_control_blocks[mbxid].mbxatr;
}

void* fake_mailbox_getLastMail(ID mbxid) {
  return mailbox_control_blocks[mbxid].last_mail;
}

bool fake_mailbox_isCreated(ID mbxid) {
  return mailbox_control_blocks[mbxid].used;
}

static ID RetrieveUnusedId(void) {
  for (ID i = kLowestId; i <= kHighestId; ++i)
    if (!mailbox_control_blocks[i].used) return i;
  return kNonsenseId;
}

ID tk_cre_mbx(CONST T_CMBX* pk_cmbx) {
  if (!pk_cmbx) return E_PAR;

  ID mbxid = RetrieveUnusedId();
  if (mbxid == kNonsenseId) return E_LIMIT;

  mailbox_control_blocks[mbxid].used = true;
  mailbox_control_blocks[mbxid].mbxatr = pk_cmbx->mbxatr;
  return mbxid;
}

ER tk_del_mbx(ID mbxid) {
  if (mbxid < kLowestId || mbxid > kHighestId) return E_ID;
  if (!mailbox_control_blocks[mbxid].used) return E_NOEXS;

  InitBlock(mbxid);
  return E_OK;
}

ER tk_snd_mbx(ID mbxid, T_MSG* pk_msg) {
  if (mbxid < kLowestId || mbxid > kHighestId) return E_ID;
  if (!mailbox_control_blocks[mbxid].used) return E_NOEXS;
  if (!pk_msg) return E_PAR;

  mailbox_control_blocks[mbxid].last_mail = (void*)pk_msg;
  return E_OK;
}

ER tk_rcv_mbx(ID mbxid, T_MSG** ppk_msg, TMO tmout) {
  if (mbxid < kLowestId || mbxid > kHighestId) return E_ID;
  if (!mailbox_control_blocks[mbxid].used) return E_NOEXS;
  if (tmout < TMO_FEVR) return E_PAR;
  if (!mailbox_control_blocks[mbxid].last_mail) return E_TMOUT;

  *ppk_msg = mailbox_control_blocks[mbxid].last_mail;
  mailbox_control_blocks[mbxid].last_mail = NULL;
  return E_OK;
}
