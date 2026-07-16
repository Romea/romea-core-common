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
#include "romea_core_common/geodesy/ECEFConverter.hpp"

inline void checkConversions(
  const romea::core::GeodeticCoordinates & llh,
  const Eigen::Vector3d & ecef)
{
  romea::core::ECEFConverter ecefConverter;
  Eigen::Vector3d ecefFromLLH = ecefConverter.toECEF(llh);
  romea::core::GeodeticCoordinates llhFromECEF = ecefConverter.toWGS84(ecefFromLLH);

  EXPECT_NEAR(llh.longitude, llhFromECEF.longitude, 0.00000001);
  EXPECT_NEAR(llh.latitude, llhFromECEF.latitude, 0.00000001);
  EXPECT_NEAR(llh.altitude, llhFromECEF.altitude, 0.01);
  EXPECT_NEAR(ecef[0], ecefFromLLH[0], 0.01);
  EXPECT_NEAR(ecef[1], ecefFromLLH[1], 0.01);
  EXPECT_NEAR(ecef[2], ecefFromLLH[2], 0.01);
}

//-----------------------------------------------------------------------------
TEST(testECEFConverter, checkConversionAtClermont)
{
  auto llh = romea::core::makeGeodeticCoordinates(45.78 / 180 * M_PI, 3.08 / 180 * M_PI, 365);
  Eigen::Vector3d ecef(4449694.95, 239429.10, 4548489.04);
  checkConversions(llh, ecef);
}

//-----------------------------------------------------------------------------
TEST(testECEFConverter, checkConversionAtMelbourne)
{
  auto llh = romea::core::makeGeodeticCoordinates(-37 / 180. * M_PI, 144.96 / 180. * M_PI, 10.);
  Eigen::Vector3d ecef(-4175633.10, 2928156.31, -3817399.17);
  checkConversions(llh, ecef);
}

//-----------------------------------------------------------------------------
TEST(testECEFConverter, checkConversionAtAnchorage)
{
  auto llh = romea::core::makeGeodeticCoordinates(61.17 / 180 * M_PI, -150.02 / 180 * M_PI, 31.);
  Eigen::Vector3d ecef(-2670982.26, -1540849.44, 5564529.01);
  checkConversions(llh, ecef);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
