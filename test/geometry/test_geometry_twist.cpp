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
#include "romea_core_common/geometry/Twist3D.hpp"
#include "test_geometry_utils.hpp"


//-----------------------------------------------------------------------------
TEST(TestTwist2D, checkDefaultConstructor)
{
  romea::core::Twist2D twist2d;

  EXPECT_DOUBLE_EQ(twist2d.angularSpeed, 0);
  EXPECT_TRUE(twist2d.linearSpeeds.isZero());
  EXPECT_TRUE(twist2d.covariance.isZero());
}

//-----------------------------------------------------------------------------
TEST(TestTwist3D, checkDefaultConstructor)
{
  romea::core::Twist3D twist3d;

  EXPECT_TRUE(twist3d.linearSpeeds.isZero());
  EXPECT_TRUE(twist3d.angularSpeeds.isZero());
  EXPECT_TRUE(twist3d.covariance.isZero());
}

//-----------------------------------------------------------------------------
TEST(TestTwist3D, checkConvertToTwist2D)
{
  romea::core::Twist3D twist3d;
  fillEigenVector(twist3d.linearSpeeds, 1);
  fillEigenVector(twist3d.angularSpeeds, 4);
  fillEigenCovariance(twist3d.covariance);

  romea::core::Twist2D twist2d = romea::core::toTwist2D(twist3d);

  isSameTwist2D(twist2d, twist3d);
}

//-----------------------------------------------------------------------------
TEST(TestTwist3D, checkConvertToTwist2DReference)
{
  romea::core::Twist3D twist3d;
  fillEigenVector(twist3d.linearSpeeds, 1);
  fillEigenVector(twist3d.angularSpeeds, 4);
  fillEigenCovariance(twist3d.covariance);

  romea::core::Twist2D twist2d;
  romea::core::toTwist2D(twist3d, twist2d);

  isSameTwist2D(twist2d, twist3d);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
