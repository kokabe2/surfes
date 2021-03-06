﻿// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_REGISTRY_INCLUDE_MUTABLE_REGISTRY_H_
#define BOOT_REGISTRY_INCLUDE_MUTABLE_REGISTRY_H_

enum Runlevel {
  kDefaultRunlevel = 3,
};

enum FileAddress {
  kCoreFileAddress = 0xFFC00000,
  kUpdaterFileAddress = 0xFFFE0000,
  kRecoverySystemFileAddress = kUpdaterFileAddress,
  kUpdateSystemFileAddress = kUpdaterFileAddress,
  kNormalSystemFileAddress = kCoreFileAddress,
  kAdvancedSystemFileAddress = kCoreFileAddress,
};

#endif  // BOOT_REGISTRY_INCLUDE_MUTABLE_REGISTRY_H_
