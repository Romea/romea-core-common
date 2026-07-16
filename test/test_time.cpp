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
#include "romea_core_common/time/Time.hpp"

//-----------------------------------------------------------------------------
TEST(TestTime, conversion)
{
  romea::core::Duration sec(1000000000);

  EXPECT_EQ(romea::core::durationFromNanoSecond(1000000000).count(), sec.count());
  EXPECT_EQ(romea::core::durationFromMicroSecond(1000000).count(), sec.count());
  EXPECT_EQ(romea::core::durationFromMilliSecond(1000).count(), sec.count());
  EXPECT_EQ(romea::core::durationFromSecond(1).count(), sec.count());

  EXPECT_EQ(romea::core::durationToNanoSecond(sec), 1000000000);
  EXPECT_EQ(romea::core::durationToMicroSecond(sec), 1000000);
  EXPECT_EQ(romea::core::durationToMilliSecond(sec), 1000);
  EXPECT_EQ(romea::core::durationToSecond(sec), 1);
}

//-----------------------------------------------------------------------------
TEST(TestTime, makeDuration)
{
  EXPECT_EQ(romea::core::makeDuration(1, 3, 43.9).count(), 3823900000000);
  EXPECT_EQ(romea::core::makeDuration(24, 0, 1.).count(), 86401000000000);
}

//-----------------------------------------------------------------------------
TEST(TestTime, durationToString)
{
  EXPECT_STREQ(
    romea::core::asString(romea::core::durationFromSecond(3823.1000)).c_str(), "01:03:43.9");
  EXPECT_STREQ(
    romea::core::asString(romea::core::durationFromSecond(86401)).c_str(), "24:00:01.0");
}

//-----------------------------------------------------------------------------
TEST(TestTime, DISABLED_makeTimePoint)
{
  EXPECT_EQ(
    romea::core::makeTimePoint(1977, 7, 17, 4, 30, 7.7).time_since_epoch().count(),
    237954607000000000);
  EXPECT_EQ(
    romea::core::makeTimePoint(2016, 3, 12, 8, 20, 50.7).time_since_epoch().count(),
    1457767250000000000);
}

//-----------------------------------------------------------------------------
TEST(TestTime, timePointToString)
{
  EXPECT_STREQ(
    romea::core::asString(romea::core::makeTimePoint(1977, 7, 17, 4, 30, 7.7)).c_str(),
    "Sun Jul 17 04:30:07 1977");
  EXPECT_STREQ(
    romea::core::asString(romea::core::makeTimePoint(2016, 3, 12, 8, 20, 50.7)).c_str(),
    "Sat Mar 12 08:20:50 2016");
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
