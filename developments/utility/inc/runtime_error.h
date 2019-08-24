// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef UTILITY_INCLUDE_RUNTIME_ERROR_H_
#define UTILITY_INCLUDE_RUNTIME_ERROR_H_

void RuntimeError(const char *message, int paramter, const char *file,
                  int line);
#define RUNTIME_ERROR(description, parameter) \
  RuntimeError(description, parameter, __FILE__, __LINE__)

#endif  // UTILITY_INCLUDE_RUNTIME_ERROR_H_
