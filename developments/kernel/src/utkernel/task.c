﻿// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "task.h"

#include <stdbool.h>
#include <stddef.h>

#include "impl/utkernel.h"
#include "instance_helper.h"

enum {
  kPriorityOffset = 5,
};

typedef int (*ResumeFunction)(int id);
typedef struct TaskStruct {
  FunctionEntry Entry;
  int id;
  ResumeFunction Resume;
} TaskStruct;

static bool Validate(FunctionEntry entry, int priority, int stack_size) {
  if (!entry) return false;
  if (priority < kTpLowestPriority || priority > kTpHighestPriority)
    return false;
  if (stack_size <= 0 || stack_size > kTssMaxSize) return false;

  return true;
}

static void TaskEntry(int start_code, void* exinf) {
  Task self = (Task)exinf;
  self->Entry(self, start_code);
  tk_exd_tsk();
}

static int ReversePriority(int priority) {
  return kTpHighestPriority - priority + kPriorityOffset;
}

static bool CreateTask(Task self, int priority, int stack_size) {
  T_CTSK packet = {
      .exinf = (void*)self,
      .tskatr = (TA_HLNG | TA_RNG0),
      .task = (FP)TaskEntry,
      .itskpri = (PRI)ReversePriority(priority),
      .stksz = (SZ)stack_size,
  };
  return (self->id = tk_cre_tsk(&packet)) >= 0;
}

static Task NewInstance(FunctionEntry entry, int priority, int stack_size,
                        int start_code) {
  Task self = (Task)InstanceHelper_New(sizeof(TaskStruct));
  if (!self) return NULL;

  self->Entry = entry;
  if (CreateTask(self, priority, stack_size))
    tk_sta_tsk(self->id, start_code);
  else
    InstanceHelper_Delete(&self);

  return self;
}

Task Task_Create(FunctionEntry entry, int priority, int stack_size,
                 int start_code) {
  return Validate(entry, priority, stack_size)
             ? NewInstance(entry, priority, stack_size, start_code)
             : NULL;
}

static bool IsMyself(int id) { return id == tk_get_tid(); }

static void KillMyself(Task* self) {
  InstanceHelper_Delete(self);
  tk_exd_tsk();  // Not returned, so delete instance first.
}

static void KillOther(Task* self) {
  tk_ter_tsk((*self)->id);
  tk_del_tsk((*self)->id);
  InstanceHelper_Delete(self);
}

void Task_Destroy(Task* self) {
  if (!self || !(*self)) return;

  if (IsMyself((*self)->id))
    KillMyself(self);
  else
    KillOther(self);
}

static void Sleep(Task self) {
  self->Resume = tk_wup_tsk;
  tk_slp_tsk(TMO_FEVR);
}

static void SuspendOther(Task self) {
  if (self->Resume) return;

  self->Resume = tk_rsm_tsk;
  tk_sus_tsk(self->id);
  tk_rel_wai(self->id);  // In case of task state is Blocked
}

void Task_Suspend(Task self) {
  if (!self) return;

  if (IsMyself(self->id))
    Sleep(self);
  else
    SuspendOther(self);
}

void Task_Resume(Task self) {
  if (!self || !self->Resume) return;

  ResumeFunction Resume = self->Resume;
  self->Resume = NULL;
  Resume(self->id);
}

void Task_Delay(Task self, int time_in_milliseconds) {
  if (!self || (time_in_milliseconds <= 0)) return;
  if (self->id != tk_get_tid()) return;

  tk_dly_tsk(time_in_milliseconds);
}
