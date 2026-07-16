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
#include <string>

// romea
#include "romea_core_common/diagnostic/DiagnosticReport.hpp"

//-----------------------------------------------------------------------------
const romea::core::Diagnostic & diagnostic(
  const romea::core::DiagnosticReport & report,
  const size_t & index)
{
  return *std::next(std::cbegin(report.diagnostics), index);
}

//-----------------------------------------------------------------------------
TEST(TestDiagnosticReport, checkSetInfosValues)
{
  romea::core::DiagnosticReport report;

  romea::core::setReportInfo(report, "foo", 1.2);
  romea::core::setReportInfo(report, "bar", "hello");
  romea::core::setReportInfo(report, "baz", -2);
  romea::core::setReportInfo(report, "qux", true);

  EXPECT_STREQ(report.info.at("foo").c_str(), "1.2");
  EXPECT_STREQ(report.info.at("bar").c_str(), "hello");
  EXPECT_STREQ(report.info.at("baz").c_str(), "-2");
  EXPECT_STREQ(report.info.at("qux").c_str(), "true");
}

//-----------------------------------------------------------------------------
TEST(TestDiagnosticReport, checkSetReportOptionalInfosValues)
{
  romea::core::DiagnosticReport report;
  std::optional<double> foo(7.77);
  std::optional<std::string> bar;

  romea::core::setReportInfo(report, "foo", foo);
  romea::core::setReportInfo(report, "bar", bar);

  EXPECT_STREQ(report.info.at("foo").c_str(), "7.77");
  EXPECT_STREQ(report.info.at("bar").c_str(), "");
}

//-----------------------------------------------------------------------------
TEST(TestDiagnosticReport, checkAggregation)
{
  romea::core::DiagnosticReport report1;
  report1.diagnostics.emplace_back(romea::core::DiagnosticStatus::OK, std::string("foo"));
  report1.info["foo"] = "0.1";

  romea::core::DiagnosticReport report2;
  report2.diagnostics.emplace_back(romea::core::DiagnosticStatus::OK, "bar");
  report2.diagnostics.emplace_back(romea::core::DiagnosticStatus::ERROR, "baz");
  report2.info["bar"] = "enable";
  report2.info["baz"] = "disable";

  romea::core::DiagnosticReport report3;
  report3 += report1;
  report3 += report2;

  EXPECT_EQ(diagnostic(report3, 0).status, romea::core::DiagnosticStatus::OK);
  EXPECT_EQ(diagnostic(report3, 0).message, "foo");
  EXPECT_EQ(diagnostic(report3, 1).status, romea::core::DiagnosticStatus::OK);
  EXPECT_EQ(diagnostic(report3, 1).message, "bar");
  EXPECT_EQ(diagnostic(report3, 2).status, romea::core::DiagnosticStatus::ERROR);
  EXPECT_EQ(diagnostic(report3, 2).message, "baz");
  EXPECT_STREQ(report3.info["foo"].c_str(), "0.1");
  EXPECT_STREQ(report3.info["bar"].c_str(), "enable");
  EXPECT_STREQ(report3.info["baz"].c_str(), "disable");
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
