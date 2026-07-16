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
#include "romea_core_common/diagnostic/CheckupEqualTo.hpp"


//-----------------------------------------------------------------------------
TEST(TestCheckupEqualTo, checkValueIsTooHigh)
{
  romea::core::CheckupEqualTo<double> checkup("foo", 1.0, 0.1);
  EXPECT_EQ(checkup.evaluate(1.2), romea::core::DiagnosticStatus::ERROR);
  EXPECT_STREQ(checkup.getReport().info.at("foo").c_str(), "1.2");
  EXPECT_STREQ(checkup.getReport().diagnostics.front().message.c_str(), "foo is too high.");
}

//-----------------------------------------------------------------------------
TEST(TestCheckupEqualTo, checkValueIsOK)
{
  romea::core::CheckupEqualTo<double> checkup("foo", 1.0, 0.1);
  EXPECT_EQ(checkup.evaluate(1.05), romea::core::DiagnosticStatus::OK);
  EXPECT_STREQ(checkup.getReport().info.at("foo").c_str(), "1.05");
  EXPECT_STREQ(checkup.getReport().diagnostics.front().message.c_str(), "foo is OK.");
}

//-----------------------------------------------------------------------------
TEST(TestCheckupEqualTo, checkValueIsTooLow)
{
  romea::core::CheckupEqualTo<double> checkup("foo", 1.0, 0.1);
  EXPECT_EQ(checkup.evaluate(0.8), romea::core::DiagnosticStatus::ERROR);
  EXPECT_STREQ(checkup.getReport().info.at("foo").c_str(), "0.8");
  EXPECT_STREQ(checkup.getReport().diagnostics.front().message.c_str(), "foo is too low.");
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
