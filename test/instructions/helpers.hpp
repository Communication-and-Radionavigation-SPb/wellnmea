#pragma once

#include <gtest/gtest.h>


template<class Instruction>
inline void test_clonable(){
  Instruction* instr = new Instruction("base");

  auto clone = instr->clone("clone");

  EXPECT_EQ(clone->name(), "clone");
  EXPECT_NE(clone, instr);

  delete instr;
  delete clone;
}