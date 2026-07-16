// Copyright 2022 INRAE, French National Research Institute for Agriculture, Food and Environment
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


// gtest
#include <gtest/gtest.h>

// romea
#include "romea_core_common/diagnostic/CheckupGreaterThan.hpp"


//-----------------------------------------------------------------------------
TEST(TestCheckupGreaterThan, checkValueIsOK)
{
  romea::core::CheckupGreaterThan<double> checkup("foo", 1.0, 0.1);
  EXPECT_EQ(checkup.evaluate(0.95), romea::core::DiagnosticStatus::OK);
  EXPECT_STREQ(checkup.getReport().info.at("foo").c_str(), "0.95");
  EXPECT_STREQ(checkup.getReport().diagnostics.front().message.c_str(), "foo is OK.");
}

//-----------------------------------------------------------------------------
TEST(TestCheckupGreaterThan, checkValueAtLowerBoundaryIsNotOK)
{
  romea::core::CheckupGreaterThan<double> checkup("foo", 1.0, 0.1);
  EXPECT_EQ(checkup.evaluate(0.9), romea::core::DiagnosticStatus::ERROR);
  EXPECT_STREQ(checkup.getReport().info.at("foo").c_str(), "0.9");
  EXPECT_STREQ(checkup.getReport().diagnostics.front().message.c_str(), "foo is too low.");
}

//-----------------------------------------------------------------------------
TEST(TestCheckupGreaterThan, checkValueIsTooLow)
{
  romea::core::CheckupGreaterThan<double> checkup("foo", 1.0, 0.1);
  EXPECT_EQ(checkup.evaluate(0.85), romea::core::DiagnosticStatus::ERROR);
  EXPECT_STREQ(checkup.getReport().info.at("foo").c_str(), "0.85");
  EXPECT_STREQ(checkup.getReport().diagnostics.front().message.c_str(), "foo is too low.");
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
