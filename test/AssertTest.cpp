#include <gtest/gtest.h>
#include <pillar/Assert.hpp>
#include <iostream>

TEST(AssertTest, assertion)
{
  pillar::print_assertion("testfilename.cpp", 1,"test_func_name", "test print_assertion message");
}