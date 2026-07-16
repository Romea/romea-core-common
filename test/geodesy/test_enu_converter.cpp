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
#include "romea_core_common/geodesy/ENUConverter.hpp"

//-----------------------------------------------------------------------------
inline void checkConversions(
  const romea::core::GeodeticCoordinates & llh,
  const Eigen::Vector3d & enu)
{
  romea::core::ENUConverter enuConverter(llh);

  romea::core::GeodeticCoordinates llhFromENU = enuConverter.toWGS84(enu);
  Eigen::Vector3d enuFromLLH = enuConverter.toENU(llhFromENU);

  EXPECT_NEAR(enu[0], enuFromLLH[0], 0.01);
  EXPECT_NEAR(enu[1], enuFromLLH[1], 0.01);
  EXPECT_NEAR(enu[2], enuFromLLH[2], 0.01);
}

//-----------------------------------------------------------------------------
TEST(testENUConverter, checkConversions)
{
  auto llh = romea::core::makeGeodeticCoordinates(-37 / 180. * M_PI, 144.96 / 180. * M_PI, 10);
  checkConversions(llh, Eigen::Vector3d::Zero());
  checkConversions(llh, Eigen::Vector3d(1, -4, 10));
  checkConversions(llh, Eigen::Vector3d(-0.4, 2, -7.8));
}

//-----------------------------------------------------------------------------
TEST(testENUConverter, checkAnchorState)
{
  auto llh = romea::core::makeGeodeticCoordinates(-37 / 180. * M_PI, 144.96 / 180. * M_PI, 10);

  romea::core::ENUConverter enuConverter;
  EXPECT_FALSE(enuConverter.isAnchored());

  enuConverter.setAnchor(llh);
  EXPECT_TRUE(enuConverter.isAnchored());
  EXPECT_DOUBLE_EQ(enuConverter.getAnchor().latitude, llh.latitude);
  EXPECT_DOUBLE_EQ(enuConverter.getAnchor().longitude, llh.longitude);
  EXPECT_DOUBLE_EQ(enuConverter.getAnchor().altitude, llh.altitude);

  enuConverter.reset();
  EXPECT_FALSE(enuConverter.isAnchored());
  EXPECT_TRUE(
    enuConverter.getEnuToEcefIsometryTransform().isApprox(Eigen::Isometry3d::Identity(), 1e-12));
}

//-----------------------------------------------------------------------------
TEST(testENUConverter, checkLazyAnchor)
{
  auto llh = romea::core::makeGeodeticCoordinates(-37 / 180. * M_PI, 144.96 / 180. * M_PI, 10);

  romea::core::ENUConverter enuConverter;
  EXPECT_FALSE(enuConverter.isAnchored());

  Eigen::Vector3d enu = enuConverter.toENU(llh);

  EXPECT_TRUE(enuConverter.isAnchored());
  EXPECT_NEAR(enu.norm(), 0, 1e-6);
}

//-----------------------------------------------------------------------------
TEST(testENUConverter, checkConversionPrecision)
{
  auto llh1 = romea::core::makeGeodeticCoordinates(45.7800 / 180 * M_PI, 3.0800 / 180 * M_PI, 365);
  auto llh2 = romea::core::makeGeodeticCoordinates(45.7801 / 180 * M_PI, 3.0801 / 180 * M_PI, 365);
  auto llh3 = romea::core::makeGeodeticCoordinates(45.7802 / 180 * M_PI, 3.0802 / 180 * M_PI, 365);

  romea::core::ENUConverter enuConverter1(llh1);
  romea::core::ENUConverter enuConverter2(llh2);
  romea::core::ENUConverter enuConverter3(llh3);

  EXPECT_NEAR(0, (enuConverter2.toENU(llh1) + enuConverter2.toENU(llh3)).norm(), 0.01);
  EXPECT_NEAR(
    0, (enuConverter1.toENU(llh2) + enuConverter1.toENU(llh3) +
    enuConverter3.toENU(llh1) + enuConverter3.toENU(llh2)).norm(), 0.01);
}

////-----------------------------------------------------------------------------
// TEST(testGeodesy, testWGS84Distance)
//{
//  romea::GeodeticCoordinates llh1(45.7800/180*M_PI,3.0800/180*M_PI,365);
//  romea::GeodeticCoordinates llh2(45.7801/180*M_PI,3.0801/180*M_PI,365);

//  double meridionalRadius = romea::EarthEllipsoid::GRS80.meridionalRadius(llh1.getLatitude());
//  double transversalRadius = romea::EarthEllipsoid::GRS80.transversalRadius(llh1.getLatitude());
//  double dy= (llh2.getLatitude()-llh1.getLatitude())*meridionalRadius;
//  double dx= (llh2.getLongitude()-llh1.getLongitude())*transversalRadius;

//  romea::ENUConverter enuConverter1(llh1);
//  Eigen::Vector3d enu2 = enuConverter1.toENU(llh2);

//  EXPECT_NEAR(0,(Eigen::Vector3d(dx,dy,0)-enu2).norm(),0.01);
//}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
