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
#include "romea_core_common/containers/grid/GridIndexMapping.hpp"
#include "romea_core_common/containers/grid/WrappableGrid.hpp"
#include "romea_core_common/containers/grid/RayTracing.hpp"


class TestGridIndexMapping2d : public ::testing::Test
{
public:
  using PointType = romea::core::GridIndexMapping2d::PointType;
  using CellIndexes = romea::core::GridIndexMapping2d::CellIndexes;

  TestGridIndexMapping2d()
  : gridExtremities(
      PointType(-1, -2) + PointType(10.3, -6.6),
      PointType(1, 2) + PointType(10.3, -6.6)),
    gridMapping(gridExtremities, 1)
  {
  }

  romea::core::Interval2D<double> gridExtremities;
  romea::core::GridIndexMapping2d gridMapping;
};

//-----------------------------------------------------------------------------
TEST_F(TestGridIndexMapping2d, isNumberOfCellsOK)
{
  EXPECT_EQ(gridMapping.getNumberOfCellsAlongAxes().x(), 3);
  EXPECT_EQ(gridMapping.getNumberOfCellsAlongAxes().y(), 5);
}

//-----------------------------------------------------------------------------
TEST_F(TestGridIndexMapping2d, isComputeCellCenterPositionOK)
{
  {
    PointType cellCenterPosition = gridMapping.computeCellCenterPosition(CellIndexes(0, 0));
    EXPECT_NEAR(cellCenterPosition.x(), 9, 0.01);
    EXPECT_NEAR(cellCenterPosition.y(), -9, 0.01);
  }
  {
    PointType cellCenterPosition = gridMapping.computeCellCenterPosition(CellIndexes(2, 4));
    EXPECT_NEAR(cellCenterPosition.x(), 11, 0.01);
    EXPECT_NEAR(cellCenterPosition.y(), -5, 0.01);
  }
}

//-----------------------------------------------------------------------------
TEST_F(TestGridIndexMapping2d, iscomputeCellIndexesOK)
{
  CellIndexes cellIndexes = gridMapping.computeCellIndexes(PointType(10.6, -5.4));
  EXPECT_EQ(cellIndexes.x(), 2);
  EXPECT_EQ(cellIndexes.y(), 4);
}

//-----------------------------------------------------------------------------
TEST_F(TestGridIndexMapping2d, isUpdateCenterOK)
{
  {
    auto delta = gridMapping.updateCenter(PointType(10.3, -6.6));
    EXPECT_EQ(delta.x(), 0);
    EXPECT_EQ(delta.y(), 0);
  }
  {
    auto delta = gridMapping.updateCenter(PointType(10.9, -7.2));
    EXPECT_EQ(delta.x(), 1);
    EXPECT_EQ(delta.y(), 0);
  }
  {
    auto delta = gridMapping.updateCenter(PointType(9.3, -8.5));
    EXPECT_EQ(delta.x(), -2);
    EXPECT_EQ(delta.y(), -2);
  }
}

class TestGridIndexMapping3d : public ::testing::Test
{
public:
  using PointType = romea::core::GridIndexMapping3d::PointType;
  using CellIndexes = romea::core::GridIndexMapping3d::CellIndexes;

  TestGridIndexMapping3d()
  : gridExtremities(PointType(-1, -2, -3), PointType(1, 2, 3)),
    gridMapping(gridExtremities, 1)
  {
  }

  romea::core::Interval3D<double> gridExtremities;
  romea::core::GridIndexMapping3d gridMapping;
};

//-----------------------------------------------------------------------------
TEST_F(TestGridIndexMapping3d, isNumberOfCellsOK)
{
  EXPECT_EQ(gridMapping.getNumberOfCellsAlongAxes().x(), 3);
  EXPECT_EQ(gridMapping.getNumberOfCellsAlongAxes().y(), 5);
  EXPECT_EQ(gridMapping.getNumberOfCellsAlongAxes().z(), 7);
}

//-----------------------------------------------------------------------------
TEST_F(TestGridIndexMapping3d, isComputeCellCenterPositionOK)
{
  PointType cellCenterPosition = gridMapping.computeCellCenterPosition(CellIndexes(0, 0, 0));

  EXPECT_NEAR(cellCenterPosition.x(), -1, 0.01);
  EXPECT_NEAR(cellCenterPosition.y(), -2, 0.01);
  EXPECT_NEAR(cellCenterPosition.z(), -3, 0.01);
}

//-----------------------------------------------------------------------------
TEST_F(TestGridIndexMapping3d, iscomputeCellIndexesOK)
{
  CellIndexes cellIndexes = gridMapping.computeCellIndexes(PointType(0.2, 0.2, 0.2));

  EXPECT_EQ(cellIndexes.x(), 1);
  EXPECT_EQ(cellIndexes.y(), 2);
  EXPECT_EQ(cellIndexes.z(), 3);
}


//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
