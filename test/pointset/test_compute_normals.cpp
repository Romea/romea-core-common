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
#include <vector>
#include <string>

// local
#include "../test/test_helper.h"
#include "test_pointset_utils.hpp"


//-----------------------------------------------------------------------------
TEST(TestComputeNormal, checkComputeNormals2d)
{
  constexpr double normalTolerance = 0.05;
  constexpr double curvatureTolerance = 0.02;

  // load scan data and compute normals and curvatures
  romea::core::PointSet<Eigen::Vector2d> pointSet = loadScan<Eigen::Vector2d>("/scan2d.txt");

  romea::core::NormalSet<Eigen::Vector2d> normalSet(pointSet.size());
  std::vector<double> curvatures(pointSet.size());
  std::vector<double> reliabilities(pointSet.size());
  romea::core::NormalAndCurvatureEstimation<Eigen::Vector2d> nche(20);
  nche.compute(pointSet, normalSet, curvatures, reliabilities);

  // load theorical normal and curvature values and compare with estimated values
  std::ifstream data(std::string(TEST_DIR) + std::string("/normals2d.txt"));

  ASSERT_TRUE(data.is_open());
  size_t n = 0;
  double nx, ny, c, r;
  while (data >> nx >> ny >> c >> r) {
    SCOPED_TRACE(n);
    ASSERT_LT(n, normalSet.size());
    EXPECT_NEAR(nx, normalSet[n][0], normalTolerance);
    EXPECT_NEAR(ny, normalSet[n][1], normalTolerance);
    EXPECT_NEAR(c, curvatures[n], curvatureTolerance);
    // EXPECT_NEAR(r,reliabilities[n],0.01);
    // std::cout << nx <<" "<< ny <<" "<< c <<" "<< r << std::endl;
    n++;
  }
  EXPECT_EQ(n, normalSet.size());
}

//-----------------------------------------------------------------------------
TEST(TestComputeNormal, checkComputeNormals3d)
{
  constexpr double normalTolerance = 0.05;
  constexpr double curvatureTolerance = 0.02;

  // load scan data and compute normals and curvatures
  romea::core::PointSet<Eigen::Vector3d> pointSet =
    loadScan<Eigen::Vector3d>("/scan3d.txt");

  romea::core::NormalSet<Eigen::Vector3d> normalSet(pointSet.size());
  std::vector<double> curvatures(pointSet.size());
  std::vector<double> reliabilities(pointSet.size());
  romea::core::NormalAndCurvatureEstimation<Eigen::Vector3d> nche(20);
  nche.compute(pointSet, normalSet, curvatures, reliabilities);

  // load theorical normal and curvature values and compare with estimated values
  std::ifstream data(std::string(TEST_DIR) + std::string("/normals3d.txt"));

  ASSERT_TRUE(data.is_open());
  size_t n = 0;
  double nx, ny, nz, c, r;
  while (data >> nx >> ny >> nz >> c >> r) {
    SCOPED_TRACE(n);
    ASSERT_LT(n, normalSet.size());
    EXPECT_NEAR(nx, normalSet[n][0], normalTolerance);
    EXPECT_NEAR(ny, normalSet[n][1], normalTolerance);
    EXPECT_NEAR(nz, normalSet[n][2], normalTolerance);
    EXPECT_NEAR(c, curvatures[n], curvatureTolerance);
    // EXPECT_NEAR(r,reliabilities[n],0.01);
    // std::cout << nx <<" "<< ny <<" "<< c <<" "<< r << std::endl;
    n++;
  }
  EXPECT_EQ(n, normalSet.size());
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
