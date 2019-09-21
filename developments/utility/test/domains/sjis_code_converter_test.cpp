// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "sjis_code_converter.h"
}

TEST(CharacterCodeConverterTest, ConvertSpace) {
  EXPECT_EQ(0x2121, SjisCodeConverter_ToJis(0x8140));
}

TEST(CharacterCodeConverterTest, ConvertSomeKanji) {
  EXPECT_EQ(0x3121, SjisCodeConverter_ToJis(0x8940));  // 院
  EXPECT_EQ(0x4160, SjisCodeConverter_ToJis(0x9180));  // 操
  EXPECT_EQ(0x5E7E, SjisCodeConverter_ToJis(0x9FFC));  // 滌
  EXPECT_EQ(0x3021, SjisCodeConverter_ToJis(0x889F));  // 亜
  EXPECT_EQ(0x4F53, SjisCodeConverter_ToJis(0x9872));  // 腕
  EXPECT_EQ(0x5121, SjisCodeConverter_ToJis(0x9940));  // 僉
  EXPECT_EQ(0x5F21, SjisCodeConverter_ToJis(0xE040));  // 漾
  EXPECT_EQ(0x7C7E, SjisCodeConverter_ToJis(0xEEFC));  // ＂
  EXPECT_EQ(0x797E, SjisCodeConverter_ToJis(0xED9E));  // 德
  EXPECT_EQ(0x4D5F, SjisCodeConverter_ToJis(0x977E));  // 欲
}

TEST(CharacterCodeConverterTest, ConvertOneByteCharacterWithFirstByte) {
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0x0000));
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0x0089));
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0x7F00));
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0xA100));
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0xDF00));
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0x1FAA));
}

TEST(CharacterCodeConverterTest, ConvertWithUnusedCodeOfFirstByte) {
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0x8012));
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0xA034));
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0xF056));
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0xFF78));
}

TEST(CharacterCodeConverterTest, ConvertWithUnusedCodeOfSecondByte) {
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0x8100));
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0x9F3F));
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0xE07F));
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0xEFFD));
  EXPECT_EQ(0, SjisCodeConverter_ToJis(0x90FF));
}
