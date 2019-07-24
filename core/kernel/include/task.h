// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef CORE_KERNEL_INCLUDE_TASK_H_
#define CORE_KERNEL_INCLUDE_TASK_H_

enum TaskPriority {
  kTpLowestPriority = 1,
  kTpDefaultPriority = 4,
  kTpHighestPriority = 8,
};

enum TaskStackSize {
  kTssDefaultSize = 1024,
  kTssMaxSize = 4 * 1024,
};

typedef struct TaskStruct* Task;
typedef void (*FunctionEntry)(Task self, int start_code);

Task Task_Create(FunctionEntry entry, int priority, int stack_size,
                 int start_code);
void Task_Destroy(Task* self);
void Task_Suspend(Task self);
void Task_Resume(Task self);
void Task_Delay(Task self, int time_in_milliseconds);

#endif  // CORE_KERNEL_INCLUDE_TASK_H_
