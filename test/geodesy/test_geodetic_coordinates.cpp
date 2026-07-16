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

// std
#include <cmath>

// romea
#include "romea_core_common/geodesy/EarthEllipsoid.hpp"
#include "romea_core_common/geodesy/GeodeticCoordinates.hpp"


//-----------------------------------------------------------------------------
TEST(testEarthEllipsoid, checkConstructor)
{
  romea::core::EarthEllipsoid ellipsoid(6378137.0, 6356752.314);

  EXPECT_DOUBLE_EQ(ellipsoid.a, 6378137.0);
  EXPECT_DOUBLE_EQ(ellipsoid.b, 6356752.314);
  EXPECT_DOUBLE_EQ(ellipsoid.e2, (ellipsoid.a * ellipsoid.a - ellipsoid.b * ellipsoid.b) /
    (ellipsoid.a * ellipsoid.a));
  EXPECT_DOUBLE_EQ(ellipsoid.e, std::sqrt(ellipsoid.e2));
}

//-----------------------------------------------------------------------------
TEST(testEarthEllipsoid, checkRadiiAtEquator)
{
  const romea::core::EarthEllipsoid ellipsoid = romea::core::EarthEllipsoid::GRS80;

  EXPECT_DOUBLE_EQ(ellipsoid.meridionalRadius(0), ellipsoid.b * ellipsoid.b / ellipsoid.a);
  EXPECT_DOUBLE_EQ(ellipsoid.transversalRadius(0), ellipsoid.a);
}

//-----------------------------------------------------------------------------
TEST(testWGS84Coordinates, checkFactory)
{
  const auto coordinates = romea::core::makeWGS84Coordinates(0.1, 0.2);

  EXPECT_DOUBLE_EQ(coordinates.latitude, 0.1);
  EXPECT_DOUBLE_EQ(coordinates.longitude, 0.2);
}

//-----------------------------------------------------------------------------
TEST(testGeodeticCoordinates, checkFactoryFromValues)
{
  const auto coordinates = romea::core::makeGeodeticCoordinates(0.1, 0.2, 3);

  EXPECT_DOUBLE_EQ(coordinates.latitude, 0.1);
  EXPECT_DOUBLE_EQ(coordinates.longitude, 0.2);
  EXPECT_DOUBLE_EQ(coordinates.altitude, 3);
}

//-----------------------------------------------------------------------------
TEST(testGeodeticCoordinates, checkFactoryFromWGS84Coordinates)
{
  const auto wgs84Coordinates = romea::core::makeWGS84Coordinates(0.1, 0.2);

  const auto coordinates = romea::core::makeGeodeticCoordinates(wgs84Coordinates, 3);

  EXPECT_DOUBLE_EQ(coordinates.latitude, wgs84Coordinates.latitude);
  EXPECT_DOUBLE_EQ(coordinates.longitude, wgs84Coordinates.longitude);
  EXPECT_DOUBLE_EQ(coordinates.altitude, 3);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
