// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_UTILITIES_INSTANCE_HELPER_H_
#define BOOT_BOOT_SRC_UTILITIES_INSTANCE_HELPER_H_

void* InstanceHelper_New(int size);
void InstanceHelper_Del(void** instance);
#define InstanceHelper_Delete(x) InstanceHelper_Del((void**)x)

#endif  // BOOT_BOOT_SRC_UTILITIES_INSTANCE_HELPER_H_
