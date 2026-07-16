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
#include "romea_core_common/monitoring/RateMonitoring.hpp"

//-----------------------------------------------------------------------------
TEST(TestMonitoring, rate)
{
  double expectedRate = 10;
  romea::core::RateMonitoring rateMonitoring(expectedRate);

  EXPECT_DOUBLE_EQ(rateMonitoring.update(romea::core::durationFromSecond(0)), 0.);

  for (size_t n = 1; n < 2 * static_cast<size_t>(expectedRate); ++n) {
    romea::core::Duration stamp = romea::core::durationFromSecond(n / expectedRate);
    EXPECT_DOUBLE_EQ(rateMonitoring.update(stamp), 0.);
  }

  EXPECT_DOUBLE_EQ(
    rateMonitoring.update(romea::core::durationFromSecond(2)),
    expectedRate);
  EXPECT_DOUBLE_EQ(rateMonitoring.getRate(), expectedRate);
}

//-----------------------------------------------------------------------------
TEST(TestMonitoring, timeout)
{
  romea::core::RateMonitoring rateMonitoring(10);

  EXPECT_DOUBLE_EQ(rateMonitoring.update(romea::core::durationFromSecond(0)), 0.);
  EXPECT_FALSE(rateMonitoring.timeout(romea::core::durationFromSecond(0.49)));
  EXPECT_TRUE(rateMonitoring.timeout(romea::core::durationFromSecond(0.51)));
  EXPECT_DOUBLE_EQ(rateMonitoring.getRate(), 0.);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
