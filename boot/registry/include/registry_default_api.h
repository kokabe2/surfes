// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_REGISTRY_INCLUDE_REGISTRY_DEFAULT_API_H_
#define BOOT_REGISTRY_INCLUDE_REGISTRY_DEFAULT_API_H_

enum Runlevel {
  kDefaultRunlevel = 3,
  kLowestRunlevel = 0,
  kHighestRunlevel = 6,
  kNumOfRunlevels = 7,
  kHaltModeRunlevel = 0,
  kRecoveryModeRunlevel = 1,
  kUpdateModeRunlevel,
  kUserModeRunlevel,
  kDeveloperModeRunlevel,
  kRebootModeRunlevel = 6,
};

enum FileAddress {
  kCoreFileAddress = 0xFFC00000,
  kUpdaterFileAddress = 0xFFFE0000,
};

#endif  // BOOT_REGISTRY_INCLUDE_REGISTRY_DEFAULT_API_H_
