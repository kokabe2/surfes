// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "sys_exe_file.h"

#include <stddef.h>

#include "sif_file_base.h"

enum ValidationErrors {
  kNoError = 0,
  kFileTypeError,
  kEntryPointError,
};

typedef int (*executeFunction)(int);
typedef struct SysExeFileStruct {
  SifFileBaseStruct base;
  executeFunction Execute;
} SysExeFileStruct;

static int Validate(SifHeader header) {
  if (header->type != kStExe) return kFileTypeError;
  if (!header->entry_point) return kEntryPointError;

  return kNoError;
}

static void SetField(SifHeader header, SysExeFile self) {
  self->Execute = (executeFunction)header->entry_point;
}

SysExeFile SysExeFile_Open(uintptr_t file_address) {
  if (Validate((SifHeader)file_address)) return NULL;

  SysExeFile self =
      (SysExeFile)SifFileBase_Open(file_address, sizeof(SysExeFileStruct));
  if (self) SetField((SifHeader)file_address, self);

  return self;
}

void SysExeFile_Close(SysExeFile *self) {
  SifFileBase_Close((SifFileBase *)self);
}

int SysExeFile_Execute(SysExeFile self, int parameter) {
  if (self) return self->Execute(parameter);

  return 0;
}
