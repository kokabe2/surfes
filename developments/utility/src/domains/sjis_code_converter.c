// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "sjis_code_converter.h"

#include <stdbool.h>

enum {
  kNonsenseByte = 0,
  kNonsenseCode = 0,
};

inline static bool WithinFirstByteRange(uint8_t c) {
  return (c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xEF);
}

static uint8_t getFisrtByte(uint16_t code) {
  uint8_t c = (uint8_t)(code >> 8);
  return WithinFirstByteRange(c) ? c : kNonsenseByte;
}

inline static bool WithinSecondtByteRange(uint8_t c) {
  return c >= 0x40 && c <= 0xFC && c != 0x7F;
}

static uint8_t getSecondByte(uint16_t code) {
  uint8_t c = (uint8_t)code;
  return WithinSecondtByteRange(c) ? c : kNonsenseByte;
}

inline static bool Validate(uint8_t first_byte, uint8_t second_byte) {
  return (first_byte != kNonsenseByte) && (second_byte != kNonsenseByte);
}

inline static bool FirstHalf(uint8_t first_byte) { return first_byte <= 0x9F; }

inline static bool OddSequence(uint8_t second_byte) {
  return second_byte <= 0x9E;
}

static uint8_t ToJisInFirstByte(uint8_t first_byte, uint8_t second_byte) {
  int subtrahend = FirstHalf(first_byte) ? 0x70 : 0xB0;
  int offset = OddSequence(second_byte) ? -1 : 0;
  return (first_byte - subtrahend) * 2 + offset;
}

inline static bool FirstHalfOfOddSequence(uint8_t second_byte) {
  return second_byte <= 0x7E;
}

static uint8_t ToJisInSecondByte(uint8_t second_byte) {
  int subtrahend = OddSequence(second_byte) ? 0x20 : 0x7E;
  int offset = FirstHalfOfOddSequence(second_byte) ? 1 : 0;
  return second_byte - subtrahend + offset;
}

inline static uint16_t Format(uint8_t first_byte, uint8_t second_byte) {
  return (first_byte << 8) + second_byte;
}

uint16_t SjisCodeConverter_ToJis(uint16_t code) {
  uint8_t first_byte = getFisrtByte(code);
  uint8_t second_byte = getSecondByte(code);
  if (!Validate(first_byte, second_byte)) return kNonsenseCode;

  uint8_t jis_first_byte = ToJisInFirstByte(first_byte, second_byte);
  uint8_t jis_second_byte = ToJisInSecondByte(second_byte);
  return Format(jis_first_byte, jis_second_byte);
}
