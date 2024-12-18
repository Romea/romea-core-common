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


//-----------------------------------------------------------------------------
class TestGrid2d : public ::testing::Test
{
public:
  using CellIndexes = romea::core::Grid<int, 2>::CellIndexes;

  TestGrid2d()
  : grid(CellIndexes(3, 3))
  {
  }

  void SetUp() override
  {
    int n = 0;
    for (size_t yi = 0; yi < 3; ++yi) {
      for (size_t xi = 0; xi < 3; ++xi) {
        grid(CellIndexes(xi, yi)) = n++;
      }
    }
  }

  romea::core::Grid<int, 2> grid;
};

//-----------------------------------------------------------------------------
TEST_F(TestGrid2d, checkDataBufferStorage)
{
  for (size_t n = 0; n < 9; ++n) {
    EXPECT_EQ(grid.getBuffer()[n], n);
  }
}

//-----------------------------------------------------------------------------
TEST_F(TestGrid2d, setDefaultValue)
{
  grid.setValue(1);

  for (const auto & cellValue : grid.getBuffer()) {
    EXPECT_EQ(cellValue, 1);
  }
}

//-----------------------------------------------------------------------------
class TestGrid3d : public ::testing::Test
{
public:
  using CellIndexes = romea::core::Grid<int, 3>::CellIndexes;

  TestGrid3d()
  : grid(CellIndexes(3, 3, 3))
  {
  }

  void SetUp() override
  {
    int n = 0;
    for (size_t zi = 0; zi < 3; ++zi) {
      for (size_t yi = 0; yi < 3; ++yi) {
        for (size_t xi = 0; xi < 3; ++xi) {
          grid(CellIndexes(xi, yi, zi)) = n++;
        }
      }
    }
  }

  romea::core::Grid<int, 3> grid;
};

//-----------------------------------------------------------------------------
TEST_F(TestGrid3d, checkDataBufferStorage)
{
  for (size_t n = 0; n < 27; ++n) {
    EXPECT_EQ(grid.getBuffer()[n], n);
  }
}

//-----------------------------------------------------------------------------
TEST_F(TestGrid3d, setDefaultValue)
{
  grid.setValue(1);

  for (const auto & cellValue : grid.getBuffer()) {
    EXPECT_EQ(cellValue, 1);
  }
}

//-----------------------------------------------------------------------------
TEST(TestContainers, testCircularGrid2D)
{
  using CellIndexes = romea::core::WrappableGrid<int, 2>::CellIndexes;
  using CellIndexesOffset = romea::core::WrappableGrid<int, 2>::CellIndexesOffset;

  romea::core::WrappableGrid<int, 2> grid(CellIndexes(3, 3));

  int n = 0;
  for (size_t yi = 0; yi < 3; ++yi) {
    for (size_t xi = 0; xi < 3; ++xi) {
      grid(CellIndexes(xi, yi)) = n++;
    }
  }

  int deltax = 1;
  int deltay = -1;

  grid.translate(CellIndexesOffset(deltax, deltay));

  EXPECT_EQ(grid(CellIndexes(0, 1)), 1);
  EXPECT_EQ(grid(CellIndexes(1, 1)), 2);
  EXPECT_EQ(grid(CellIndexes(0, 2)), 4);
  EXPECT_EQ(grid(CellIndexes(1, 2)), 5);
}

//-----------------------------------------------------------------------------
class TestCircularGrid3D : public ::testing::Test
{
public:
  using CellIndexes = romea::core::WrappableGrid<int, 3>::CellIndexes;
  using CellIndexesOffset = romea::core::WrappableGrid<int, 3>::CellIndexesOffset;

  TestCircularGrid3D()
  : grid(CellIndexes(3, 3, 3))
  {
  }

  void SetUp() override
  {
    int n = 0;
    for (size_t zi = 0; zi < 3; ++zi) {
      for (size_t yi = 0; yi < 3; ++yi) {
        for (size_t xi = 0; xi < 3; ++xi) {
          grid(CellIndexes(xi, yi, zi)) = n++;
        }
      }
    }
  }

  romea::core::WrappableGrid<int, 3> grid;
};

//-----------------------------------------------------------------------------
TEST_F(TestCircularGrid3D, testGridValuesAfterTranslate)
{
  grid.translate(CellIndexesOffset(1, -1, 2));

  EXPECT_EQ(grid(CellIndexes(0, 1, 0)), 19);
  EXPECT_EQ(grid(CellIndexes(0, 2, 0)), 22);
  EXPECT_EQ(grid(CellIndexes(1, 1, 0)), 20);
  EXPECT_EQ(grid(CellIndexes(1, 2, 0)), 23);
}

//-----------------------------------------------------------------------------
TEST_F(TestCircularGrid3D, isGetIndexOffsetOK)
{
  grid.translate(CellIndexesOffset(1, -1, 4));

  EXPECT_EQ(grid.getIndexOffsetAlongAxes().x(), 1);
  EXPECT_EQ(grid.getIndexOffsetAlongAxes().y(), 2);
  EXPECT_EQ(grid.getIndexOffsetAlongAxes().z(), 1);
}

//-----------------------------------------------------------------------------
TEST_F(TestCircularGrid3D, hasEmptyValuesAfterTranslate)
{
  grid.translate(CellIndexesOffset(3, -3, 3));

  for (size_t zi = 0; zi < 3; ++zi) {
    for (size_t yi = 0; yi < 3; ++yi) {
      for (size_t xi = 0; xi < 3; ++xi) {
        EXPECT_DOUBLE_EQ(grid(CellIndexes(xi, yi, zi)), 0.);
      }
    }
  }
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
