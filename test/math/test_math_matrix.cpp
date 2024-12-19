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
#include "romea_core_common/math/Matrix.hpp"


//-----------------------------------------------------------------------------
TEST(TestMatrix, testPositiveSemiDefiniteMatrix)
{
  Eigen::Matrix3d se2Covariance;
  se2Covariance.row(0) << 0.1, 0.3, -0.4;
  se2Covariance.row(1) << 0.3, 0.2, 0.3;
  se2Covariance.row(2) << -0.4, 0.3, 0.5;
  EXPECT_TRUE(romea::core::isPositiveSemiDefiniteMatrix(se2Covariance));

  Eigen::Matrix3d badSe2Covariance1 = se2Covariance;
  badSe2Covariance1(0, 1) = 0.2;
  EXPECT_FALSE(romea::core::isPositiveSemiDefiniteMatrix(badSe2Covariance1));

  Eigen::Matrix3d badSe2Covariance2 = se2Covariance;
  badSe2Covariance2(0, 2) = 0.4;
  EXPECT_FALSE(romea::core::isPositiveSemiDefiniteMatrix(badSe2Covariance2));

  Eigen::Matrix3d badSe2Covariance3 = se2Covariance;
  badSe2Covariance3(0, 0) = -0.1;
  EXPECT_FALSE(romea::core::isPositiveSemiDefiniteMatrix(badSe2Covariance3));
}

//-----------------------------------------------------------------------------
TEST(TestMatrix, testSe2ToSe3Covariance)
{
  Eigen::Matrix3d se2Covariance;
  se2Covariance.row(0) << 0.1, 0.3, -0.4;
  se2Covariance.row(1) << 0.3, 0.2, 0.3;
  se2Covariance.row(2) << -0.4, 0.3, 0.5;
  auto se3Covariance = romea::core::toSe3Covariance(se2Covariance);

  EXPECT_DOUBLE_EQ(se3Covariance(0, 0), se2Covariance(0, 0));
  EXPECT_DOUBLE_EQ(se3Covariance(0, 1), se2Covariance(0, 1));
  EXPECT_DOUBLE_EQ(se3Covariance(0, 2), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(0, 3), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(0, 4), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(0, 5), se2Covariance(0, 2));

  EXPECT_DOUBLE_EQ(se3Covariance(1, 0), se2Covariance(1, 0));
  EXPECT_DOUBLE_EQ(se3Covariance(1, 1), se2Covariance(1, 1));
  EXPECT_DOUBLE_EQ(se3Covariance(1, 2), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(1, 3), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(1, 4), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(1, 5), se2Covariance(1, 2));

  EXPECT_DOUBLE_EQ(se3Covariance(2, 0), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(2, 1), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(2, 2), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(2, 3), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(2, 4), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(2, 5), 0.);

  EXPECT_DOUBLE_EQ(se3Covariance(3, 0), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(3, 1), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(3, 2), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(3, 3), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(3, 4), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(3, 5), 0.);

  EXPECT_DOUBLE_EQ(se3Covariance(4, 0), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(4, 1), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(4, 2), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(4, 3), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(4, 4), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(4, 5), 0.);

  EXPECT_DOUBLE_EQ(se3Covariance(5, 0), se2Covariance(2, 0));
  EXPECT_DOUBLE_EQ(se3Covariance(5, 1), se2Covariance(2, 1));
  EXPECT_DOUBLE_EQ(se3Covariance(5, 2), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(5, 3), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(5, 4), 0.);
  EXPECT_DOUBLE_EQ(se3Covariance(5, 5), se2Covariance(2, 2));
}

//-----------------------------------------------------------------------------
TEST(TestMatrix, testSe3ToSe2Covariance)
{
  Eigen::Matrix6d se3Covariance;
  se3Covariance.row(0) << 0.1, 0.3, -0.4, 0.5, -0.6, 1.2;
  se3Covariance.row(1) << 0.3, 0.2, 0.3, 0.1, -0.3, 0.7;
  se3Covariance.row(2) << -0.4, 0.3, 0.5, -0.6, 0.2, 0.8;
  se3Covariance.row(3) << 0.5, 0.1, -0.6, 0.7, -0.1, 0.9;
  se3Covariance.row(4) << -0.6, -0.3, 0.2, -0.1, 0.3, -0.4;
  se3Covariance.row(5) << 1.2, 0.7, 0.8, 0.9, 0.4, 0.6;

  auto se2Covariance = romea::core::toSe2Covariance(se3Covariance);

  EXPECT_DOUBLE_EQ(se3Covariance(0, 0), se2Covariance(0, 0));
  EXPECT_DOUBLE_EQ(se3Covariance(0, 1), se2Covariance(0, 1));
  EXPECT_DOUBLE_EQ(se3Covariance(0, 5), se2Covariance(0, 2));

  EXPECT_DOUBLE_EQ(se3Covariance(1, 0), se2Covariance(1, 0));
  EXPECT_DOUBLE_EQ(se3Covariance(1, 1), se2Covariance(1, 1));
  EXPECT_DOUBLE_EQ(se3Covariance(1, 5), se2Covariance(1, 2));

  EXPECT_DOUBLE_EQ(se3Covariance(5, 0), se2Covariance(2, 0));
  EXPECT_DOUBLE_EQ(se3Covariance(5, 1), se2Covariance(2, 1));
  EXPECT_DOUBLE_EQ(se3Covariance(5, 5), se2Covariance(2, 2));
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
