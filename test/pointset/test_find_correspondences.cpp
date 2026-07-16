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
#include <algorithm>
#include <string>
#include <vector>

// local
#include "../test/test_helper.h"
#include "test_pointset_utils.hpp"

//-----------------------------------------------------------------------------
template<class PointType>
size_t findCorrespondences(const std::string & dataFileName)
{
  // load data
  romea::core::PointSet<PointType> pointSet = loadScan<PointType>(dataFileName);
  romea::core::KdTree<PointType> kdTree(pointSet);

  // Search nearest point;
  size_t nearestIndex;
  double nearestNeighborSquareDistance;
  std::vector<romea::core::Correspondence> correspondences;

  // Find correpondences
  for (size_t n = 0; n < pointSet.size(); ++n) {
    kdTree.findNearestNeighbor(pointSet[n], nearestIndex, nearestNeighborSquareDistance);
    correspondences.emplace_back(nearestIndex, n, nearestNeighborSquareDistance);
  }

  // Remove wrong correspondences
  std::vector<romea::core::Correspondence>::iterator itEnd;
  std::sort(
    std::begin(correspondences),
    std::end(correspondences),
    romea::core::sortBySourceIndexAndDistancePredicate);

  itEnd = std::unique(
    std::begin(correspondences),
    std::end(correspondences),
    romea::core::equalSourceIndexesPredicate);

  return static_cast<size_t>(std::distance(std::begin(correspondences), itEnd));
}

//-----------------------------------------------------------------------------
TEST(TestFindCorrespondences, find2DCorrespondences)
{
  EXPECT_EQ(702, findCorrespondences<Eigen::Vector2d>("/scan2d.txt"));
}

//-----------------------------------------------------------------------------
TEST(TestKdTree, findNearestNeighbor2D)
{
  romea::core::PointSet<Eigen::Vector2d> pointSet = {
    Eigen::Vector2d(0., 0.),
    Eigen::Vector2d(2., 0.),
    Eigen::Vector2d(0., 3.),
    Eigen::Vector2d(5., 5.)};
  romea::core::KdTree<Eigen::Vector2d> kdTree(pointSet);

  size_t nearestIndex = pointSet.size();
  double nearestNeighborSquareDistance = -1.;
  kdTree.findNearestNeighbor(Eigen::Vector2d(1.8, 0.1), nearestIndex, nearestNeighborSquareDistance);

  EXPECT_EQ(nearestIndex, 1u);
  EXPECT_NEAR(nearestNeighborSquareDistance, 0.05, 1e-12);
}

//-----------------------------------------------------------------------------
TEST(TestKdTree, findNearestNeighbors3D)
{
  romea::core::PointSet<Eigen::Vector3d> pointSet = {
    Eigen::Vector3d(0., 0., 0.),
    Eigen::Vector3d(1., 0., 0.),
    Eigen::Vector3d(0., 1., 0.),
    Eigen::Vector3d(0., 0., 2.)};
  romea::core::KdTree<Eigen::Vector3d> kdTree(pointSet);

  std::vector<size_t> nearestIndexes(2);
  std::vector<double> nearestNeighborSquareDistances(2);
  kdTree.findNearestNeighbors(
    Eigen::Vector3d(0., 0., 0.1),
    2,
    nearestIndexes,
    nearestNeighborSquareDistances);

  EXPECT_EQ(nearestIndexes[0], 0u);
  EXPECT_NEAR(nearestNeighborSquareDistances[0], 0.01, 1e-12);
  EXPECT_NE(
    std::find(nearestIndexes.begin(), nearestIndexes.end(), 1u),
    nearestIndexes.end());
}

//-----------------------------------------------------------------------------
TEST(TestKdTree, radiusResearchReturnsNeighborsInsideSquaredRadius)
{
  romea::core::PointSet<Eigen::Vector2d> pointSet = {
    Eigen::Vector2d(0., 0.),
    Eigen::Vector2d(0.5, 0.),
    Eigen::Vector2d(0., 0.75),
    Eigen::Vector2d(2., 0.)};
  romea::core::KdTree<Eigen::Vector2d> kdTree(pointSet);

  std::vector<size_t> neighborIndexes;
  std::vector<double> neighborSquareDistances;
  kdTree.radiusResearch(
    Eigen::Vector2d(0., 0.),
    1.,
    neighborIndexes,
    neighborSquareDistances);

  std::sort(neighborIndexes.begin(), neighborIndexes.end());
  EXPECT_EQ(neighborIndexes, std::vector<size_t>({0u, 1u, 2u}));
  EXPECT_EQ(neighborSquareDistances.size(), 3u);
}

//-----------------------------------------------------------------------------
TEST(TestFindCorrespondences, find3DCorrespondences)
{
  EXPECT_EQ(66332, findCorrespondences<Eigen::Vector3d>("/scan3d.txt"));
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
