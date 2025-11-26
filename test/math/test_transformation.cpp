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
#include "romea_core_common/math/EulerAngles.hpp"
#include "romea_core_common/math/Transformation.hpp"


//-----------------------------------------------------------------------------
TEST(TestTransformation, makeRigidTransformation)
{

  Eigen::Vector3d xyz(1,2,3);
  Eigen::Vector3d rpy(0.1, 0.2 ,0.3);
  auto H = romea::core::rigid_transformation3<double>(xyz, rpy);

  EXPECT_DOUBLE_EQ(H.translation()(0),1.0);
  EXPECT_DOUBLE_EQ(H.translation()(1),2.0);
  EXPECT_DOUBLE_EQ(H.translation()(2),3.0);

  auto I = romea::core::eulerAnglesToRotation3D(rpy).inverse()*H.rotation();
  EXPECT_DOUBLE_EQ(I(0,0),1.0);
  EXPECT_DOUBLE_EQ(I(1,1),1.0);
  EXPECT_DOUBLE_EQ(I(2,2),1.0);


}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}