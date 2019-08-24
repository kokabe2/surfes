// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef DEVELOPMENTS_UTILITY_INC_RUNTIME_ERROR_H_
#define DEVELOPMENTS_UTILITY_INC_RUNTIME_ERROR_H_

void RuntimeError(const char *message, int paramter, const char *file,
                  int line);
#define RUNTIME_ERROR(description, parameter) \
  RuntimeError(description, parameter, __FILE__, __LINE__)

#endif  // DEVELOPMENTS_UTILITY_INC_RUNTIME_ERROR_H_
