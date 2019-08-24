// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_UTILITY_INC_INSTANCE_HELPER_H_
#define DEVELOPMENTS_UTILITY_INC_INSTANCE_HELPER_H_

void* InstanceHelper_New(int size);
void InstanceHelper_Del(void** instance);
#define InstanceHelper_Delete(x) InstanceHelper_Del((void**)x)

#endif  // DEVELOPMENTS_UTILITY_INC_INSTANCE_HELPER_H_
