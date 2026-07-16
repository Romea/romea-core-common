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

// std
#include <list>
#include <sstream>

// romea
#include "romea_core_common/diagnostic/Diagnostic.hpp"

//-----------------------------------------------------------------------------
TEST(TestDiagnosticStatus, checkComparison)
{
  EXPECT_TRUE(romea::core::DiagnosticStatus::OK < romea::core::DiagnosticStatus::WARN);
  EXPECT_TRUE(romea::core::DiagnosticStatus::WARN < romea::core::DiagnosticStatus::ERROR);
  EXPECT_TRUE(romea::core::DiagnosticStatus::ERROR < romea::core::DiagnosticStatus::STALE);
}

//-----------------------------------------------------------------------------
TEST(TestDiagnosticStatus, checkWorseStatus)
{
  using romea::core::DiagnosticStatus;

  EXPECT_EQ(worse(DiagnosticStatus::OK, DiagnosticStatus::OK), DiagnosticStatus::OK);
  EXPECT_EQ(worse(DiagnosticStatus::OK, DiagnosticStatus::WARN), DiagnosticStatus::WARN);
  EXPECT_EQ(worse(DiagnosticStatus::ERROR, DiagnosticStatus::WARN), DiagnosticStatus::ERROR);
  EXPECT_EQ(worse(DiagnosticStatus::STALE, DiagnosticStatus::ERROR), DiagnosticStatus::STALE);
}

//-----------------------------------------------------------------------------
TEST(TestDiagnosticStatus, checkDiagnosticListStatus)
{
  using romea::core::Diagnostic;
  using romea::core::DiagnosticStatus;

  EXPECT_TRUE(
    romea::core::allOK(
      {Diagnostic(DiagnosticStatus::OK, "foo"), Diagnostic(DiagnosticStatus::OK, "bar")}));
  EXPECT_FALSE(
    romea::core::allOK(
      {Diagnostic(DiagnosticStatus::OK, "foo"), Diagnostic(DiagnosticStatus::WARN, "bar")}));
  EXPECT_EQ(
    romea::core::worseStatus(
      {Diagnostic(DiagnosticStatus::OK, "foo"),
        Diagnostic(DiagnosticStatus::ERROR, "bar"),
        Diagnostic(DiagnosticStatus::WARN, "baz")}),
    DiagnosticStatus::ERROR);
}

//-----------------------------------------------------------------------------
TEST(TestDiagnosticStatus, checkToStringConversion)
{
  EXPECT_STREQ(toString(romea::core::DiagnosticStatus::OK).c_str(), "OK");
  EXPECT_STREQ(toString(romea::core::DiagnosticStatus::WARN).c_str(), "WARN");
  EXPECT_STREQ(toString(romea::core::DiagnosticStatus::ERROR).c_str(), "ERROR");
  EXPECT_STREQ(toString(romea::core::DiagnosticStatus::STALE).c_str(), "STALE");
}

//-----------------------------------------------------------------------------
TEST(TestDiagnosticStatus, checkStreamConversion)
{
  std::ostringstream statusStream;
  statusStream << romea::core::DiagnosticStatus::WARN;
  EXPECT_EQ(statusStream.str(), "WARN");

  std::ostringstream diagnosticStream;
  diagnosticStream << romea::core::Diagnostic(romea::core::DiagnosticStatus::ERROR, "foo failed");
  EXPECT_EQ(diagnosticStream.str(), "ERROR : foo failed");
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
