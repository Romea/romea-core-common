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
#include "romea_core_common/geometry/Position2D.hpp"
#include "romea_core_common/geometry/Position3D.hpp"


//-----------------------------------------------------------------------------
TEST(TestPosition2D, checkDefaultConstructor)
{
  romea::core::Position2D position2d;

  EXPECT_TRUE(position2d.position.isZero());
  EXPECT_TRUE(position2d.covariance.isZero());
}

//-----------------------------------------------------------------------------
TEST(TestPosition2D, checkComputeEllipse)
{
  romea::core::Position2D position2d;
  position2d.position.x() = 1;
  position2d.position.y() = 2;
  position2d.covariance(0, 0) = 9;
  position2d.covariance(1, 1) = 4;

  romea::core::Ellipse ellipse = romea::core::uncertaintyEllipse(position2d, 2);

  EXPECT_DOUBLE_EQ(ellipse.getCenterPosition().x(), position2d.position.x());
  EXPECT_DOUBLE_EQ(ellipse.getCenterPosition().y(), position2d.position.y());
  EXPECT_NEAR(ellipse.getOrientation(), 0, 1e-12);
  EXPECT_DOUBLE_EQ(ellipse.getMajorRadius(), 6);
  EXPECT_DOUBLE_EQ(ellipse.getMinorRadius(), 4);
}

//-----------------------------------------------------------------------------
TEST(TestPosition3D, checkDefaultConstructor)
{
  romea::core::Position3D position3d;

  EXPECT_TRUE(position3d.position.isZero());
  EXPECT_TRUE(position3d.covariance.isZero());
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
