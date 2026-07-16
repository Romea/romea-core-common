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
#include "romea_core_common/diagnostic/CheckupReliability.hpp"

class TestCheckupReliability : public ::testing::Test
{
public:
  TestCheckupReliability()
  : diagnostic("foo", 0.7, 0.9)
  {
  }

  romea::core::CheckupReliability diagnostic;
};

//-----------------------------------------------------------------------------
TEST_F(TestCheckupReliability, compareWithLowReliability)
{
  EXPECT_EQ(diagnostic.evaluate(0.6), romea::core::DiagnosticStatus::ERROR);
  EXPECT_EQ(
    diagnostic.getReport().diagnostics.front().status,
    romea::core::DiagnosticStatus::ERROR);
  EXPECT_STREQ(diagnostic.getReport().diagnostics.front().message.c_str(), "foo is too low.");
  EXPECT_STREQ(diagnostic.getReport().info.at("foo").c_str(), "0.6");
}

//-----------------------------------------------------------------------------
TEST_F(TestCheckupReliability, compareAtLowReliabilityThreshold)
{
  EXPECT_EQ(diagnostic.evaluate(0.7), romea::core::DiagnosticStatus::WARN);
  EXPECT_EQ(diagnostic.getReport().diagnostics.front().status, romea::core::DiagnosticStatus::WARN);
  EXPECT_STREQ(diagnostic.getReport().diagnostics.front().message.c_str(), "foo is uncertain.");
  EXPECT_STREQ(diagnostic.getReport().info.at("foo").c_str(), "0.7");
}

//-----------------------------------------------------------------------------
TEST_F(TestCheckupReliability, compareWithUncertainReliability)
{
  EXPECT_EQ(diagnostic.evaluate(0.75), romea::core::DiagnosticStatus::WARN);
  EXPECT_EQ(diagnostic.getReport().diagnostics.front().status, romea::core::DiagnosticStatus::WARN);
  EXPECT_STREQ(diagnostic.getReport().diagnostics.front().message.c_str(), "foo is uncertain.");
  EXPECT_STREQ(diagnostic.getReport().info.at("foo").c_str(), "0.75");
}

//-----------------------------------------------------------------------------
TEST_F(TestCheckupReliability, compareAtHighReliabilityThreshold)
{
  EXPECT_EQ(diagnostic.evaluate(0.9), romea::core::DiagnosticStatus::OK);
  EXPECT_EQ(diagnostic.getReport().diagnostics.front().status, romea::core::DiagnosticStatus::OK);
  EXPECT_STREQ(diagnostic.getReport().diagnostics.front().message.c_str(), "foo is high.");
  EXPECT_STREQ(diagnostic.getReport().info.at("foo").c_str(), "0.9");
}

//-----------------------------------------------------------------------------
TEST_F(TestCheckupReliability, compareWithHighReliability)
{
  EXPECT_EQ(diagnostic.evaluate(0.95), romea::core::DiagnosticStatus::OK);
  EXPECT_EQ(diagnostic.getReport().diagnostics.front().status, romea::core::DiagnosticStatus::OK);
  EXPECT_STREQ(diagnostic.getReport().diagnostics.front().message.c_str(), "foo is high.");
  EXPECT_STREQ(diagnostic.getReport().info.at("foo").c_str(), "0.95");
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
