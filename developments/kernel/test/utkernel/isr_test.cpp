// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "impl/fake_int.h"
#include "isr.h"
}

namespace {
bool isr_ran;
int given_no;
void FakeIsr(int no) {
  given_no = no;
  isr_ran = true;
}
}  // namespace

class IsrTest : public ::testing::Test {
 protected:
  Isr instance;

  virtual void SetUp() {
    fake_int_init();
    isr_ran = false;
    given_no = -1;
    instance = Isr_Create(24, FakeIsr);
  }
  virtual void TearDown() { Isr_Destroy(&instance); }
};

TEST_F(IsrTest, Create) {
  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ(TA_HLNG, fake_int_getAttribute(24));
  EXPECT_EQ((FP)FakeIsr, fake_int_getIsrFunction(24));
}

TEST_F(IsrTest, CreateWithInvalidInterruptNumber) {
  EXPECT_EQ(NULL, Isr_Create(12345678, FakeIsr));
  EXPECT_EQ(NULL, Isr_Create(-1, FakeIsr));
}

TEST_F(IsrTest, Destroy) {
  Isr_Destroy(&instance);

  EXPECT_EQ(NULL, instance);
  EXPECT_NE((FP)FakeIsr, fake_int_getIsrFunction(24));
}

TEST_F(IsrTest, DestroyAfterDestroy) {
  Isr_Destroy(&instance);
  Isr_Destroy(&instance);

  SUCCEED();
}

TEST_F(IsrTest, DestroyWithNull) {
  Isr_Destroy(NULL);

  SUCCEED();
}

TEST_F(IsrTest, Enable) {
  Isr_Enable(instance, 4);
  fake_int_run(24);

  EXPECT_TRUE(isr_ran);
  EXPECT_EQ(24, given_no);
  EXPECT_EQ(4, fake_int_getLevel(24));
}

TEST_F(IsrTest, EnableWithNull) {
  Isr_Enable(NULL, 4);

  SUCCEED();
}

TEST_F(IsrTest, Disable) {
  Isr_Enable(instance, 4);

  Isr_Disable(instance);
  fake_int_run(24);

  EXPECT_FALSE(isr_ran);
}

TEST_F(IsrTest, DisableWithNull) {
  Isr_Disable(NULL);

  SUCCEED();
}
