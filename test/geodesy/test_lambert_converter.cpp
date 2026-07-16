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
#include "romea_core_common/geodesy/LambertConverter.hpp"

//-----------------------------------------------------------------------------
TEST(testLambertConverter, computeIsometricLatitude)
{
  double e = 0.08199188998;
  double eps = 1e-11;
  EXPECT_NEAR(
    romea::core::LambertConverter::computeIsometricLatitude(0.87266462600, e),
    1.00552653649, eps);

  EXPECT_NEAR(
    romea::core::LambertConverter::computeIsometricLatitude(-0.3, e),
    -0.30261690063, eps);

  EXPECT_NEAR(
    romea::core::LambertConverter::computeIsometricLatitude(0.19998903370, e),
    0.200000000009, eps);
}

//-----------------------------------------------------------------------------
TEST(testLambertConverter, Latitude)
{
  double e = 0.08199188998;
  EXPECT_NEAR(
    romea::core::LambertConverter::computeLatitude(1.00552653648, e),
    0.87266462600, 1e-11);
  EXPECT_NEAR(
    romea::core::LambertConverter::computeLatitude(-0.30261690060, e),
    -0.29999999997, 1e-11);
  EXPECT_NEAR(
    romea::core::LambertConverter::computeLatitude(0.2, e),
    0.19998903369, 1e-11);
}

//-----------------------------------------------------------------------------
TEST(testLambertConverter, computeGrandNormal)
{
  double a = 6378388;
  double e = 0.081991890;
  double latitude = 0.97738438100;
  double eps = 0.0001;
  EXPECT_NEAR(
    romea::core::LambertConverter::computeGrandeNormal(latitude, a, e), 6393174.9755,
    eps);
}

//-----------------------------------------------------------------------------
TEST(testLambertConverter, computeParametersFromTangentProjectionParemeters)
{
  {
    romea::core::LambertConverter::TangentProjectionParameters sp;
    sp.longitude0 = 0.18112808800;
    sp.latitude0 = 0.97738438100;
    sp.k0 = 1.;
    sp.x0 = 0.;
    sp.y0 = 0.;

    romea::core::EarthEllipsoid e(6378388, 6356911.946116942);

    auto p = romea::core::LambertConverter::computeProjectionParameters(sp, e);
    EXPECT_DOUBLE_EQ(p.longitude0, sp.longitude0);
    EXPECT_NEAR(p.n, 0.8290375725, 1e-09);
    EXPECT_NEAR(p.c, 11464828.2192, 0.001);
    EXPECT_DOUBLE_EQ(p.xs, sp.x0);
    EXPECT_NEAR(p.ys, 4312250.9718, 0.001);
  }

  {
    romea::core::LambertConverter::TangentProjectionParameters sp;
    sp.longitude0 = 0.04079234433;
    sp.latitude0 = 0.86393798000;
    sp.k0 = 0.9998773400;
    sp.x0 = 600000.;
    sp.y0 = 200000.;

    romea::core::EarthEllipsoid e(6378249.200, 6356514.999980688);

    auto p = romea::core::LambertConverter::computeProjectionParameters(sp, e);
    EXPECT_DOUBLE_EQ(p.longitude0, sp.longitude0);
    EXPECT_NEAR(p.n, 0.7604059658, 1e-09);
    EXPECT_NEAR(p.c, 11603796.9760, 0.001);
    EXPECT_DOUBLE_EQ(p.xs, sp.x0);
    EXPECT_NEAR(p.ys, 5657616.6712, 0.001);
  }
}
//-----------------------------------------------------------------------------
TEST(testLambertConverter, computeParametersFromSecantProjectionParemeters)
{
  romea::core::EarthEllipsoid e(6378388, 6356911.946116942);

  {
    romea::core::LambertConverter::SecantProjectionParameters sp;
    sp.longitude0 = 0.;
    sp.latitude0 = 0.;
    sp.latitude1 = -0.57595865300;
    sp.latitude2 = -0.78539816300;
    sp.x0 = 0.;
    sp.y0 = 0.;

    auto p = romea::core::LambertConverter::computeProjectionParameters(sp, e);
    EXPECT_DOUBLE_EQ(p.longitude0, sp.longitude0);
    EXPECT_NEAR(p.n, -0.6304963300, 1e-09);
    EXPECT_NEAR(p.c, -12453174.1795, 0.001);
    EXPECT_DOUBLE_EQ(p.xs, sp.x0);
    EXPECT_NEAR(p.ys, -12453174.1795, 0.001);
  }

  {
    romea::core::LambertConverter::SecantProjectionParameters sp;
    sp.longitude0 = 0.07623554539;
    sp.latitude0 = 1.57079632700;
    sp.latitude1 = 0.869755744;
    sp.latitude2 = 0.893026801;
    sp.x0 = 150000.;
    sp.y0 = 5400000.;

    auto p = romea::core::LambertConverter::computeProjectionParameters(sp, e);
    EXPECT_DOUBLE_EQ(p.longitude0, sp.longitude0);
    EXPECT_NEAR(p.n, 0.7716421867, 1e-09);
    EXPECT_NEAR(p.c, 11565915.8294, 0.001);
    EXPECT_DOUBLE_EQ(p.xs, sp.x0);
    EXPECT_DOUBLE_EQ(p.ys, sp.y0);
  }
}

//-----------------------------------------------------------------------------
TEST(testLambertConverter, toLambert)
{
  romea::core::LambertConverter converter(
    0.0407923443,
    0.760405966,
    11603796.9767,
    600000.,
    5657616.6740,
    0.0824832568);

  romea::core::WGS84Coordinates wgs84Coordinates;
  wgs84Coordinates.latitude = 0.8726646260;
  wgs84Coordinates.longitude = 0.14551209900;


  auto position = converter.toLambert(wgs84Coordinates);
  EXPECT_NEAR(position.x(), 1029705.0818, 0.001);
  EXPECT_NEAR(position.y(), 272723.8510, 0.001);
}

//-----------------------------------------------------------------------------
TEST(testLambertConverter, toLambertCC46)
{
  romea::core::LambertConverter::SecantProjectionParameters parameters;
  parameters.longitude0 = 3.0 / 180. * M_PI;
  parameters.latitude0 = 46.0 / 180. * M_PI;
  parameters.latitude1 = 45.25 / 180. * M_PI;
  parameters.latitude2 = 46.75 / 180. * M_PI;
  parameters.x0 = 1700000.;
  parameters.y0 = 5200000.;

  romea::core::LambertConverter converter(parameters, romea::core::EarthEllipsoid::GRS80);

  romea::core::WGS84Coordinates wgs84Coordinates;
  wgs84Coordinates.latitude = 45.693834 / 180. * M_PI;
  wgs84Coordinates.longitude = 3.100827 / 180. * M_PI;

  auto position = converter.toLambert(wgs84Coordinates);
  EXPECT_NEAR(position.x(), 1707852.76, 0.1);
  EXPECT_NEAR(position.y(), 5165977.89, 0.1);
}

//-----------------------------------------------------------------------------
TEST(testLambertConverter, toWGS84)
{
  romea::core::LambertConverter converter(
    0.04079234433,
    0.760405966,
    11603796.9767,
    600000,
    5657616.674,
    0.0824832568);

  auto wgs84Coordinates = converter.toWGS84(Eigen::Vector2d(1029705.0830, 272723.8490));
  EXPECT_NEAR(wgs84Coordinates.latitude, 0.87266462567, 1e-11);
  EXPECT_NEAR(wgs84Coordinates.longitude, 0.14551209925, 1e-11);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
