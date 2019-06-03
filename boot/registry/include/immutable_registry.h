// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_REGISTRY_INCLUDE_IMMUTABLE_REGISTRY_H_
#define BOOT_REGISTRY_INCLUDE_IMMUTABLE_REGISTRY_H_

enum BootFileMetadata {
  kBootFileVersion = 0x0000000100000000,
  kBootFileAddress = 0xFFFFE000,
  kRegistryFileAddress = 0xFFFFC000,
  kRecoveryFileAddress = 0xFFFF0000,
};

enum Runlevel {
  kLowestRunlevel = 0,
  kHighestRunlevel = 6,
  kNumOfRunlevels = 7,
  kHaltModeRunlevel = 0,
  kRecoveryModeRunlevel = 1,
  kUpdateModeRunlevel,
  kNormalModeRunlevel,
  kAdvancedModeRunlevel,
  kRebootModeRunlevel = 6,
};

#endif  // BOOT_REGISTRY_INCLUDE_IMMUTABLE_REGISTRY_H_
