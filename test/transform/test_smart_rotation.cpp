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
#include <romea_core_common/transform/SmartRotation3D.hpp>

namespace
{

const double angle = 60 / 180. * M_PI;
const double cosAngle = std::cos(angle);
const double sinAngle = std::sin(angle);
const double eps = 0.0001;

//-----------------------------------------------------------------------------
Eigen::Matrix3d computeNumericalDerivative(const Eigen::Vector3d & angles, int angleIndex)
{
  constexpr double epsilon = 1e-6;

  Eigen::Vector3d forwardAngles = angles;
  Eigen::Vector3d backwardAngles = angles;
  forwardAngles(angleIndex) += epsilon;
  backwardAngles(angleIndex) -= epsilon;

  romea::core::SmartRotation3D forwardRotation(forwardAngles);
  romea::core::SmartRotation3D backwardRotation(backwardAngles);

  return (forwardRotation.R() - backwardRotation.R()) / (2 * epsilon);
}

//-----------------------------------------------------------------------------
void checkDerivatives(const Eigen::Vector3d & angles)
{
  romea::core::SmartRotation3D rotation(angles);

  EXPECT_TRUE(rotation.dRdAngleAroundXAxis().isApprox(computeNumericalDerivative(angles, 0), eps));
  EXPECT_TRUE(rotation.dRdAngleAroundYAxis().isApprox(computeNumericalDerivative(angles, 1), eps));
  EXPECT_TRUE(rotation.dRdAngleAroundZAxis().isApprox(computeNumericalDerivative(angles, 2), eps));

  Eigen::Vector3d vector(1, 2, 3);
  Eigen::Matrix3d expectedDRTdAngles;
  expectedDRTdAngles.col(0) = rotation.dRdAngleAroundXAxis() * vector;
  expectedDRTdAngles.col(1) = rotation.dRdAngleAroundYAxis() * vector;
  expectedDRTdAngles.col(2) = rotation.dRdAngleAroundZAxis() * vector;

  EXPECT_TRUE(rotation.dRTdAngles(vector).isApprox(expectedDRTdAngles, eps));
}

}  // namespace

//-----------------------------------------------------------------------------
TEST(testSmartRotation3D, checkRollRotation)
{
  romea::core::SmartRotation3D rotation(angle, 0, 0);

  Eigen::Matrix3d expectedRotation;
  expectedRotation << 1, 0, 0,
    0, cosAngle, -sinAngle,
    0, sinAngle, cosAngle;

  EXPECT_TRUE(rotation.R().isApprox(expectedRotation, eps));
  EXPECT_TRUE((rotation * Eigen::Vector3d(1, 2, 3)).isApprox(Eigen::Vector3d(1, -1.5981, 3.2321), eps));
  checkDerivatives(Eigen::Vector3d(angle, 0, 0));
}

//-----------------------------------------------------------------------------
TEST(testSmartRotation3D, checkPitchRotation)
{
  romea::core::SmartRotation3D rotation(0, angle, 0);

  Eigen::Matrix3d expectedRotation;
  expectedRotation << cosAngle, 0, sinAngle,
    0, 1, 0,
    -sinAngle, 0, cosAngle;

  EXPECT_TRUE(rotation.R().isApprox(expectedRotation, eps));
  EXPECT_TRUE((rotation * Eigen::Vector3d(1, 2, 3)).isApprox(Eigen::Vector3d(3.0981, 2, 0.6340), eps));
  checkDerivatives(Eigen::Vector3d(0, angle, 0));
}

//-----------------------------------------------------------------------------
TEST(testSmartRotation3D, checkYawRotation)
{
  romea::core::SmartRotation3D rotation(0, 0, angle);

  Eigen::Matrix3d expectedRotation;
  expectedRotation << cosAngle, -sinAngle, 0,
    sinAngle, cosAngle, 0,
    0, 0, 1;

  EXPECT_TRUE(rotation.R().isApprox(expectedRotation, eps));
  EXPECT_TRUE((rotation * Eigen::Vector3d(1, 2, 3)).isApprox(Eigen::Vector3d(-1.2321, 1.8660, 3), eps));
  checkDerivatives(Eigen::Vector3d(0, 0, angle));
}

//-----------------------------------------------------------------------------
TEST(testSmartRotation3D, checkFullRotation)
{
  romea::core::SmartRotation3D rotation(angle, angle, angle);

  Eigen::Matrix3d expectedRotation;
  expectedRotation << 0.250000, -0.058013, 0.966506,
    0.433013, 0.899519, -0.058013,
    -0.866025, 0.433013, 0.250000;

  EXPECT_TRUE(rotation.R().isApprox(expectedRotation, eps));
  EXPECT_TRUE((rotation * Eigen::Vector3d(1, 2, 3)).isApprox(Eigen::Vector3d(3.0335, 2.0580, 0.7500), eps));
  checkDerivatives(Eigen::Vector3d(angle, angle, angle));
}

//-----------------------------------------------------------------------------
TEST(testSmartRotation3D, checkDerivativesForSeveralRotations)
{
  checkDerivatives(Eigen::Vector3d(0, 0, 0));
  checkDerivatives(Eigen::Vector3d(1e-6, -1e-6, 1e-6));
  checkDerivatives(Eigen::Vector3d(0.01, -0.02, 0.03));
  checkDerivatives(Eigen::Vector3d(-0.01, 0.02, -0.03));
  checkDerivatives(Eigen::Vector3d(angle, 0, -angle));
  checkDerivatives(Eigen::Vector3d(0, -angle, angle));
  checkDerivatives(Eigen::Vector3d(-angle, angle, -angle));
  checkDerivatives(Eigen::Vector3d(angle / 2, -angle / 3, angle / 4));
  checkDerivatives(Eigen::Vector3d(-angle / 5, angle / 7, -angle / 9));
  checkDerivatives(Eigen::Vector3d(2 * angle, -angle / 2, angle / 8));
  checkDerivatives(Eigen::Vector3d(-M_PI / 2, M_PI / 3, -M_PI / 4));
  checkDerivatives(Eigen::Vector3d(M_PI / 2, -M_PI / 3, M_PI / 4));
  checkDerivatives(Eigen::Vector3d(-M_PI + 0.01, M_PI / 2 - 0.02, -M_PI / 6));
  checkDerivatives(Eigen::Vector3d(M_PI - 0.01, -M_PI / 2 + 0.02, M_PI / 6));
  checkDerivatives(Eigen::Vector3d(2 * M_PI, -2 * M_PI, M_PI));
  checkDerivatives(Eigen::Vector3d(-2 * M_PI + 0.1, 2 * M_PI - 0.2, -M_PI + 0.3));
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
