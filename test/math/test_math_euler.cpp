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

//-----------------------------------------------------------------------------
TEST(TestEuler, angleWrapping)
{
  EXPECT_DOUBLE_EQ(romea::core::between0And2Pi(0.), 0.);
  EXPECT_DOUBLE_EQ(romea::core::between0And2Pi(-0.5), romea::core::M_2PI - 0.5);
  EXPECT_DOUBLE_EQ(romea::core::between0And2Pi(romea::core::M_2PI + 0.5), 0.5);

  EXPECT_DOUBLE_EQ(romea::core::betweenMinusPiAndPi(0.), 0.);
  EXPECT_DOUBLE_EQ(romea::core::betweenMinusPiAndPi(M_PI + 0.5), -M_PI + 0.5);
  EXPECT_DOUBLE_EQ(romea::core::betweenMinusPiAndPi(-M_PI - 0.5), M_PI - 0.5);
}

//-----------------------------------------------------------------------------
TEST(TestEuler, rotation2DConversion)
{
  const double yaw = 1.2;
  const Eigen::Matrix2d rotation = romea::core::eulerAngleToRotation2D(yaw);

  EXPECT_NEAR(rotation(0, 0), std::cos(yaw), 1e-12);
  EXPECT_NEAR(rotation(0, 1), -std::sin(yaw), 1e-12);
  EXPECT_NEAR(rotation(1, 0), std::sin(yaw), 1e-12);
  EXPECT_NEAR(rotation(1, 1), std::cos(yaw), 1e-12);
  EXPECT_NEAR(romea::core::rotation2DToEulerAngle(rotation), yaw, 1e-12);
}

//-----------------------------------------------------------------------------
TEST(TestEuler, testRX)
{
  double roll = 0.8;
  Eigen::Vector3d eulerAngles(roll, 0, 0);
  Eigen::Matrix3d R = romea::core::eulerAnglesToRotation3D(eulerAngles);
  eulerAngles = romea::core::rotation3DToEulerAngles(R);

  EXPECT_NEAR(R(0, 0), 1, 0.001);
  EXPECT_NEAR(R(0, 1), 0, 0.001);
  EXPECT_NEAR(R(0, 2), 0, 0.001);
  EXPECT_NEAR(R(1, 0), 0, 0.001);
  EXPECT_NEAR(R(2, 0), 0, 0.001);

  EXPECT_NEAR(eulerAngles.x(), roll, 0.001);
  EXPECT_NEAR(eulerAngles.y(), 0, 0.001);
  EXPECT_NEAR(eulerAngles.z(), 0, 0.001);
}

//-----------------------------------------------------------------------------
TEST(TestEuler, testRY)
{
  double pitch = M_PI_2 - 0.5;
  Eigen::Vector3d eulerAngles(0, pitch, 0);
  Eigen::Matrix3d R = romea::core::eulerAnglesToRotation3D(eulerAngles);
  eulerAngles = romea::core::rotation3DToEulerAngles(R);

  EXPECT_NEAR(R(0, 1), 0, 0.001);
  EXPECT_NEAR(R(1, 0), 0, 0.001);
  EXPECT_NEAR(R(1, 1), 1, 0.001);
  EXPECT_NEAR(R(1, 2), 0, 0.001);
  EXPECT_NEAR(R(2, 1), 0, 0.001);

  EXPECT_NEAR(eulerAngles.x(), 0, 0.001);
  EXPECT_NEAR(eulerAngles.y(), pitch, 0.001);
  EXPECT_NEAR(eulerAngles.z(), 0, 0.001);
}


//-----------------------------------------------------------------------------
TEST(TestEuler, testRZ)
{
  double yaw = 1.1;
  Eigen::Vector3d eulerAngles(0, 0, yaw);
  Eigen::Matrix3d R = romea::core::eulerAnglesToRotation3D(eulerAngles);
  eulerAngles = romea::core::rotation3DToEulerAngles(R);

  EXPECT_NEAR(R(0, 2), 0, 0.001);
  EXPECT_NEAR(R(1, 2), 0, 0.001);
  EXPECT_NEAR(R(2, 0), 0, 0.001);
  EXPECT_NEAR(R(2, 1), 0, 0.001);
  EXPECT_NEAR(R(2, 2), 1, 0.001);

  EXPECT_NEAR(eulerAngles.x(), 0, 0.001);
  EXPECT_NEAR(eulerAngles.y(), 0, 0.001);
  EXPECT_NEAR(eulerAngles.z(), yaw, 0.001);
}

//-----------------------------------------------------------------------------
TEST(TestEuler, testR)
{
  double roll = 0.8;
  double pitch = M_PI_2 - 0.5;
  double yaw = 1.1;

  Eigen::Vector3d eulerAngles(roll, pitch, yaw);
  eulerAngles = romea::core::rotation3DToEulerAngles(
    romea::core::eulerAnglesToRotation3D(eulerAngles));
  EXPECT_NEAR(eulerAngles.x(), roll, 0.001);
  EXPECT_NEAR(eulerAngles.y(), pitch, 0.001);
  EXPECT_NEAR(eulerAngles.z(), yaw, 0.001);
}

//-----------------------------------------------------------------------------
TEST(TestEuler, quaternionToEulerAngles)
{
  double roll = 0.8;
  double pitch = -0.2;
  double yaw = 2.1;
  Eigen::Vector3d eulerAngles(roll, pitch, yaw);
  Eigen::Quaterniond q = Eigen::Quaterniond(
    romea::core::eulerAnglesToRotation3D(eulerAngles));
  eulerAngles = romea::core::quaternionToEulerAngles(q);
  EXPECT_NEAR(eulerAngles.x(), romea::core::between0And2Pi(roll), 0.001);
  EXPECT_NEAR(eulerAngles.y(), romea::core::between0And2Pi(pitch), 0.001);
  EXPECT_NEAR(eulerAngles.z(), romea::core::between0And2Pi(yaw), 0.001);
}

//-----------------------------------------------------------------------------
inline void testRotationPrediction(
  const Eigen::Vector3d & initialEulerAngles,
  const Eigen::Vector3d & angularSpeeds,
  const double & dt)
{
  size_t N = 1000;
  Eigen::Matrix3d rotation = romea::core::eulerAnglesToRotation3D(initialEulerAngles);

  double dti = dt / N;
  Eigen::Matrix3d F;
  Eigen::Vector3d eulerAngles = initialEulerAngles;

  for (size_t n = 0; n < N; n++) {
    F.row(0) << 1, std::sin(eulerAngles[0]) * std::tan(eulerAngles[1]),
      std::cos(eulerAngles[0]) * std::tan(eulerAngles[1]);
    F.row(1) << 0, std::cos(eulerAngles[0]), -std::sin(eulerAngles[0]);
    F.row(2) << 0, std::sin(eulerAngles[0]) / std::cos(eulerAngles[1]),
      std::cos(eulerAngles[0]) / std::cos(eulerAngles[1]);
    eulerAngles += F * angularSpeeds * dti;

    rotation = rotation *
      romea::core::eulerAnglesToRotation3D(Eigen::Vector3d(angularSpeeds * dti));
  }

  EXPECT_LT(
    romea::core::betweenMinusPiAndPi(
      eulerAngles.x() -
      romea::core::rotation3DToEulerAngles(rotation).x()), 0.01);
  EXPECT_LT(
    romea::core::betweenMinusPiAndPi(
      eulerAngles.y() -
      romea::core::rotation3DToEulerAngles(rotation).y()), 0.01);
  EXPECT_LT(
    romea::core::betweenMinusPiAndPi(
      eulerAngles.z() -
      romea::core::rotation3DToEulerAngles(rotation).z()), 0.01);
}

//-----------------------------------------------------------------------------
TEST(TestEuler, testRotationPrediction1)
{
  double dt = 1;
  Eigen::Vector3d eulerAngles(0, 0, 0);
  Eigen::Vector3d eulerAngularSpeed(0.3, -0.5, 0.1);
  testRotationPrediction(eulerAngles, eulerAngularSpeed, dt);
}

//-----------------------------------------------------------------------------
TEST(TestEuler, testRotationPrediction2)
{
  double dt = 1;
  Eigen::Vector3d eulerAngles(0.5, -.2, .3);
  Eigen::Vector3d eulerAngularSpeed(0.3, -0.5, 0.1);
  testRotationPrediction(eulerAngles, eulerAngularSpeed, dt);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
