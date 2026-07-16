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
#include <cmath>

// romea
#include "romea_core_common/monitoring/OnlineVariance.hpp"

//-----------------------------------------------------------------------------
TEST(TestMonitoring, onlineAverage)
{
  romea::core::OnlineAverage onlineAverage(0.001, 4);

  EXPECT_FALSE(onlineAverage.isAvailable());
  EXPECT_TRUE(std::isnan(onlineAverage.getAverage()));

  onlineAverage.update(1);
  onlineAverage.update(2);
  onlineAverage.update(3);
  onlineAverage.update(4);

  EXPECT_TRUE(onlineAverage.isAvailable());
  EXPECT_DOUBLE_EQ(onlineAverage.getAverage(), 2.5);

  onlineAverage.update(5);
  EXPECT_DOUBLE_EQ(onlineAverage.getAverage(), 3.5);

  onlineAverage.reset();
  EXPECT_FALSE(onlineAverage.isAvailable());
  EXPECT_TRUE(std::isnan(onlineAverage.getAverage()));
}

//-----------------------------------------------------------------------------
TEST(TestMonitoring, onlineVariance)
{
  romea::core::OnlineVariance onlineVariance(0.001, 4);

  EXPECT_FALSE(onlineVariance.isAvailable());
  EXPECT_TRUE(std::isnan(onlineVariance.getAverage()));
  EXPECT_TRUE(std::isnan(onlineVariance.getVariance()));

  onlineVariance.update(1);
  onlineVariance.update(2);
  onlineVariance.update(3);
  onlineVariance.update(4);

  EXPECT_TRUE(onlineVariance.isAvailable());
  EXPECT_DOUBLE_EQ(onlineVariance.getAverage(), 2.5);
  EXPECT_NEAR(onlineVariance.getVariance(), 5. / 3., 1e-12);

  onlineVariance.update(5);
  EXPECT_DOUBLE_EQ(onlineVariance.getAverage(), 3.5);
  EXPECT_NEAR(onlineVariance.getVariance(), 5. / 3., 1e-12);

  onlineVariance.reset();
  EXPECT_FALSE(onlineVariance.isAvailable());
  EXPECT_TRUE(std::isnan(onlineVariance.getAverage()));
  EXPECT_TRUE(std::isnan(onlineVariance.getVariance()));
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
