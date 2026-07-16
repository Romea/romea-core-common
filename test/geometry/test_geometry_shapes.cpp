// Copyright 2026 INRAE, French National Research Institute for Agriculture, Food and Environment
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
#include "romea_core_common/geometry/Circle.hpp"
#include "romea_core_common/geometry/Ellipse.hpp"


//-----------------------------------------------------------------------------
TEST(TestCircle, checkConstructorFromCoordinates)
{
  romea::core::Circle circle(1, 2, 3);

  EXPECT_DOUBLE_EQ(circle.getCenterPosition().x(), 1);
  EXPECT_DOUBLE_EQ(circle.getCenterPosition().y(), 2);
  EXPECT_DOUBLE_EQ(circle.getRadius(), 3);
}

//-----------------------------------------------------------------------------
TEST(TestCircle, checkConstructorFromVector)
{
  Eigen::Vector2d centerPosition(1, 2);

  romea::core::Circle circle(centerPosition, 3);

  EXPECT_TRUE(circle.getCenterPosition().isApprox(centerPosition, 1e-12));
  EXPECT_DOUBLE_EQ(circle.getRadius(), 3);
}

//-----------------------------------------------------------------------------
TEST(TestEllipse, checkConstructorFromCoordinates)
{
  romea::core::Ellipse ellipse(1, 2, 3, 4, 5);

  EXPECT_DOUBLE_EQ(ellipse.getCenterPosition().x(), 1);
  EXPECT_DOUBLE_EQ(ellipse.getCenterPosition().y(), 2);
  EXPECT_DOUBLE_EQ(ellipse.getOrientation(), 3);
  EXPECT_DOUBLE_EQ(ellipse.getMajorRadius(), 4);
  EXPECT_DOUBLE_EQ(ellipse.getMinorRadius(), 5);
}

//-----------------------------------------------------------------------------
TEST(TestEllipse, checkConstructorFromVector)
{
  Eigen::Vector2d centerPosition(1, 2);

  romea::core::Ellipse ellipse(centerPosition, 3, 4, 5);

  EXPECT_TRUE(ellipse.getCenterPosition().isApprox(centerPosition, 1e-12));
  EXPECT_DOUBLE_EQ(ellipse.getOrientation(), 3);
  EXPECT_DOUBLE_EQ(ellipse.getMajorRadius(), 4);
  EXPECT_DOUBLE_EQ(ellipse.getMinorRadius(), 5);
}

//-----------------------------------------------------------------------------
TEST(TestEllipse, checkConstructorFromCovariance)
{
  Eigen::Vector2d centerPosition(1, 2);
  Eigen::Matrix2d covariance = Eigen::Matrix2d::Zero();
  covariance(0, 0) = 9;
  covariance(1, 1) = 4;

  romea::core::Ellipse ellipse(centerPosition, covariance, 2);

  EXPECT_TRUE(ellipse.getCenterPosition().isApprox(centerPosition, 1e-12));
  EXPECT_NEAR(ellipse.getOrientation(), 0, 1e-12);
  EXPECT_DOUBLE_EQ(ellipse.getMajorRadius(), 6);
  EXPECT_DOUBLE_EQ(ellipse.getMinorRadius(), 4);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
