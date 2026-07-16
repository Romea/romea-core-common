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
#include "romea_core_common/geometry/Pose2D.hpp"


//-----------------------------------------------------------------------------
TEST(TestPose2D, checkDefaultConstructor)
{
  romea::core::Pose2D pose2d;

  EXPECT_DOUBLE_EQ(pose2d.yaw, 0);
  EXPECT_TRUE(pose2d.position.isZero());
  EXPECT_TRUE(pose2d.covariance.isZero());
}

//-----------------------------------------------------------------------------
TEST(TestPose2D, checkComputeEllipse)
{
  romea::core::Pose2D pose2d;
  pose2d.position.x() = 1;
  pose2d.position.y() = 2;
  pose2d.covariance(0, 0) = 4;
  pose2d.covariance(1, 1) = 1;

  romea::core::Ellipse ellipse = romea::core::uncertaintyEllipse(pose2d, 2);

  EXPECT_DOUBLE_EQ(ellipse.getCenterPosition().x(), pose2d.position.x());
  EXPECT_DOUBLE_EQ(ellipse.getCenterPosition().y(), pose2d.position.y());
  EXPECT_NEAR(ellipse.getOrientation(), 0, 1e-12);
  EXPECT_DOUBLE_EQ(ellipse.getMajorRadius(), 4);
  EXPECT_DOUBLE_EQ(ellipse.getMinorRadius(), 2);
}


//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
