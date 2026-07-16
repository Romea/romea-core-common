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
#include "romea_core_common/geometry/PoseAndTwist3D.hpp"
#include "test_geometry_utils.hpp"


//-----------------------------------------------------------------------------
TEST(TestPoseAndTwist2D, checkDefaultConstructor)
{
  romea::core::PoseAndTwist2D poseAndTwist2d;

  EXPECT_DOUBLE_EQ(poseAndTwist2d.pose.yaw, 0);
  EXPECT_TRUE(poseAndTwist2d.pose.position.isZero());
  EXPECT_TRUE(poseAndTwist2d.pose.covariance.isZero());
  EXPECT_DOUBLE_EQ(poseAndTwist2d.twist.angularSpeed, 0);
  EXPECT_TRUE(poseAndTwist2d.twist.linearSpeeds.isZero());
  EXPECT_TRUE(poseAndTwist2d.twist.covariance.isZero());
}

//-----------------------------------------------------------------------------
TEST(TestPoseAndTwist3D, checkDefaultConstructor)
{
  romea::core::PoseAndTwist3D poseAndTwist3d;

  EXPECT_TRUE(poseAndTwist3d.pose.position.isZero());
  EXPECT_TRUE(poseAndTwist3d.pose.orientation.isZero());
  EXPECT_TRUE(poseAndTwist3d.pose.covariance.isZero());
  EXPECT_TRUE(poseAndTwist3d.twist.linearSpeeds.isZero());
  EXPECT_TRUE(poseAndTwist3d.twist.angularSpeeds.isZero());
  EXPECT_TRUE(poseAndTwist3d.twist.covariance.isZero());
}

//-----------------------------------------------------------------------------
TEST(TestPoseAndTwist3D, checkConvertToPoseAndTwist2D)
{
  romea::core::PoseAndTwist3D poseAndTwist3d;
  fillEigenVector(poseAndTwist3d.pose.position, 1);
  fillEigenVector(poseAndTwist3d.pose.orientation, 4);
  fillEigenCovariance(poseAndTwist3d.pose.covariance);
  fillEigenVector(poseAndTwist3d.twist.linearSpeeds, 7);
  fillEigenVector(poseAndTwist3d.twist.angularSpeeds, 10);
  fillEigenCovariance(poseAndTwist3d.twist.covariance, 13);

  romea::core::PoseAndTwist2D poseAndTwist2d =
    romea::core::toPoseAndTwist2D(poseAndTwist3d);

  isSamePose2D(poseAndTwist2d.pose, poseAndTwist3d.pose);
  isSameTwist2D(poseAndTwist2d.twist, poseAndTwist3d.twist);
}

//-----------------------------------------------------------------------------
TEST(TestPoseAndTwist3D, checkConvertToPoseAndTwist2DReference)
{
  romea::core::PoseAndTwist3D poseAndTwist3d;
  fillEigenVector(poseAndTwist3d.pose.position, 1);
  fillEigenVector(poseAndTwist3d.pose.orientation, 4);
  fillEigenCovariance(poseAndTwist3d.pose.covariance);
  fillEigenVector(poseAndTwist3d.twist.linearSpeeds, 7);
  fillEigenVector(poseAndTwist3d.twist.angularSpeeds, 10);
  fillEigenCovariance(poseAndTwist3d.twist.covariance, 13);

  romea::core::PoseAndTwist2D poseAndTwist2d;
  romea::core::toPoseAndTwist2D(poseAndTwist3d, poseAndTwist2d);

  isSamePose2D(poseAndTwist2d.pose, poseAndTwist3d.pose);
  isSameTwist2D(poseAndTwist2d.twist, poseAndTwist3d.twist);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
