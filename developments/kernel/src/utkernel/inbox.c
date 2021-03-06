﻿// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "inbox.h"

#include <stddef.h>
#include <string.h>

#include "impl/utkernel.h"
#include "instance_helper.h"

typedef struct InboxStruct {
  int mbx_id;
  int mpl_id;
  void* got_mail;
} InboxStruct;

static const int kHeaderSize = sizeof(T_MSG);

static bool IsValid(int inbox_size) {
  return (inbox_size > 0) && (inbox_size <= kIsMaxSize);
}

static bool CreateInbox(Inbox self, int inbox_size) {
  T_CMBX mbx_packet = {
      .mbxatr = (TA_TFIFO | TA_MFIFO),
  };
  T_CMPL mpl_packet = {
      .mplatr = (TA_TFIFO | TA_RNG0),
      .mplsz = inbox_size,
  };
  return ((self->mbx_id = tk_cre_mbx(&mbx_packet)) >= 0) &&
         ((self->mpl_id = tk_cre_mpl(&mpl_packet)) >= 0);
}

static Inbox NewInstance(int inbox_size) {
  Inbox self = (Inbox)InstanceHelper_New(sizeof(InboxStruct));
  if (self && !CreateInbox(self, inbox_size)) Inbox_Destroy(&self);
  return self;
}

Inbox Inbox_Create(int inbox_size) {
  return IsValid(inbox_size) ? NewInstance(inbox_size) : NULL;
}

void Inbox_Destroy(Inbox* self) {
  if (!self || !(*self)) return;

  tk_del_mbx((*self)->mbx_id);
  tk_del_mpl((*self)->mpl_id);
  InstanceHelper_Delete(self);
}

static bool Validate(int message_size, const void* message) {
  return (message_size > 0) && message;
}

static void* NewMail(Inbox self, int message_size, TMO timeout) {
  void* mail;
  return tk_get_mpl(self->mpl_id, kHeaderSize + message_size, &mail, timeout) ==
                 E_OK
             ? mail
             : NULL;
}

static void EditMail(void* mail, int message_size, const void* message) {
  memcpy(mail + kHeaderSize, message, message_size);
}

static void SendMail(Inbox self, void* mail) {
  tk_snd_mbx(self->mbx_id, (T_MSG*)mail);
}

bool ComposeThenSend(Inbox self, int message_size, const void* message,
                     TMO timeout) {
  if (!self || !Validate(message_size, message)) return false;

  void* mail = NewMail(self, message_size, timeout);
  if (!mail) return false;

  EditMail(mail, message_size, message);
  SendMail(self, mail);
  return true;
}

bool Inbox_Post(Inbox self, int message_size, const void* message) {
  return ComposeThenSend(self, message_size, message, TMO_POL);
}

static void DeletePreviousMail(Inbox self) {
  if (!self->got_mail) return;
  tk_rel_mpl(self->mpl_id, self->got_mail);
  self->got_mail = NULL;
}

static bool GetNextMail(Inbox self, TMO timeout) {
  if (tk_rcv_mbx(self->mbx_id, (T_MSG**)&self->got_mail, timeout) != E_OK)
    self->got_mail = NULL;  // Just in case.
  return self->got_mail != NULL;
}

static void* ExtractMessage(Inbox self) { return self->got_mail + kHeaderSize; }

void* DeleteThenGet(Inbox self, TMO timeout) {
  if (!self) return NULL;

  DeletePreviousMail(self);
  return GetNextMail(self, timeout) ? ExtractMessage(self) : NULL;
}

void* Inbox_Get(Inbox self) { return DeleteThenGet(self, TMO_POL); }

bool Inbox_Send(Inbox self, int message_size, const void* message) {
  return ComposeThenSend(self, message_size, message, TMO_FEVR);
}

void* Inbox_Receive(Inbox self) { return DeleteThenGet(self, TMO_FEVR); }
