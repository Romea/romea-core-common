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
#include "romea_core_common/geometry/Pose3D.hpp"
#include "romea_core_common/math/EulerAngles.hpp"
#include "test_geometry_utils.hpp"

namespace
{

//-----------------------------------------------------------------------------
Eigen::Vector6d toVector(const romea::core::Pose3D & pose3d)
{
  Eigen::Vector6d vector;
  vector << pose3d.position, pose3d.orientation;
  return vector;
}

//-----------------------------------------------------------------------------
Eigen::Vector6d normalizedDifference(
  const romea::core::Pose3D & pose3d1,
  const romea::core::Pose3D & pose3d2)
{
  Eigen::Vector6d difference = toVector(pose3d1) - toVector(pose3d2);
  difference.tail<3>() = difference.tail<3>().unaryExpr(
    [](double angle) {return romea::core::betweenMinusPiAndPi(angle);});
  return difference;
}

//-----------------------------------------------------------------------------
Eigen::Matrix6d computeNumericalJacobian(
  const Eigen::Isometry3d & isometry,
  const romea::core::Pose3D & pose3d)
{
  constexpr double epsilon = 1e-6;

  Eigen::Matrix6d jacobian;
  for (int n = 0; n < 6; ++n) {
    romea::core::Pose3D forwardPose = pose3d;
    romea::core::Pose3D backwardPose = pose3d;

    if (n < 3) {
      forwardPose.position(n) += epsilon;
      backwardPose.position(n) -= epsilon;
    } else {
      forwardPose.orientation(n - 3) += epsilon;
      backwardPose.orientation(n - 3) -= epsilon;
    }

    jacobian.col(n) =
      normalizedDifference(isometry * forwardPose, isometry * backwardPose) / (2 * epsilon);
  }

  return jacobian;
}

}  // namespace

//-----------------------------------------------------------------------------
TEST(TestPose3D, checkDefaultConstructor)
{
  romea::core::Pose3D pose3d;

  EXPECT_TRUE(pose3d.position.isZero());
  EXPECT_TRUE(pose3d.orientation.isZero());
  EXPECT_TRUE(pose3d.covariance.isZero());
}

//-----------------------------------------------------------------------------
TEST(TestPose3D, checkConvertToPose2D)
{
  romea::core::Pose3D pose3d;
  fillEigenVector(pose3d.position, 1);
  fillEigenVector(pose3d.orientation, 4);
  fillEigenCovariance(pose3d.covariance);

  romea::core::Pose2D pose2d = romea::core::toPose2D(pose3d);

  isSamePose2D(pose2d, pose3d);
}

//-----------------------------------------------------------------------------
TEST(TestPose3D, checkConvertToPose2DReference)
{
  romea::core::Pose3D pose3d;
  fillEigenVector(pose3d.position, 1);
  fillEigenVector(pose3d.orientation, 4);
  fillEigenCovariance(pose3d.covariance);

  romea::core::Pose2D pose2d;
  romea::core::toPose2D(pose3d, pose2d);

  isSamePose2D(pose2d, pose3d);
}

//-----------------------------------------------------------------------------
TEST(TestPose3D, checkConvertToPosition3D)
{
  romea::core::Pose3D pose3d;
  fillEigenVector(pose3d.position, 1);
  fillEigenVector(pose3d.orientation, 4);
  fillEigenCovariance(pose3d.covariance);

  romea::core::Position3D position3d = romea::core::toPosition3D(pose3d);

  isSamePosition3D(position3d, pose3d);
}

//-----------------------------------------------------------------------------
TEST(TestPose3D, checkConvertToPosition3DReference)
{
  romea::core::Pose3D pose3d;
  fillEigenVector(pose3d.position, 1);
  fillEigenVector(pose3d.orientation, 4);
  fillEigenCovariance(pose3d.covariance);

  romea::core::Position3D position3d;
  romea::core::toPosition3D(pose3d, position3d);

  isSamePosition3D(position3d, pose3d);
}

//-----------------------------------------------------------------------------
TEST(TestPose3D, checkIdentityTransform)
{
  romea::core::Pose3D pose3d;
  pose3d.position << 1, 2, 3;
  pose3d.orientation << 0.1, 0.2, 0.3;
  pose3d.covariance.setZero();
  pose3d.covariance(0, 0) = 2;
  pose3d.covariance(1, 1) = 3;
  pose3d.covariance(2, 2) = 4;

  romea::core::Pose3D transformed = Eigen::Isometry3d::Identity() * pose3d;

  EXPECT_TRUE(transformed.position.isApprox(pose3d.position, 1e-12));
  EXPECT_TRUE(transformed.orientation.isApprox(pose3d.orientation, 1e-12));
  EXPECT_TRUE(transformed.covariance.isApprox(pose3d.covariance, 1e-12));
}

//-----------------------------------------------------------------------------
TEST(TestPose3D, checkTranslationTransform)
{
  romea::core::Pose3D pose3d;
  pose3d.position << 1, 2, 3;
  pose3d.orientation << 0, 0, 0;
  pose3d.covariance = Eigen::Matrix6d::Identity();

  Eigen::Isometry3d isometry = Eigen::Isometry3d::Identity();
  isometry.translation() << 4, 5, 6;

  romea::core::Pose3D transformed = isometry * pose3d;

  EXPECT_TRUE(transformed.position.isApprox(Eigen::Vector3d(5, 7, 9), 1e-12));
  EXPECT_TRUE(transformed.orientation.isApprox(pose3d.orientation, 1e-12));
  EXPECT_TRUE(transformed.covariance.isApprox(pose3d.covariance, 1e-12));
}

//-----------------------------------------------------------------------------
TEST(TestPose3D, checkTransformCovarianceWithNumericalJacobian)
{
  romea::core::Pose3D pose3d;
  pose3d.position << 1, 2, 3;
  pose3d.orientation << 0.2, -0.3, 0.4;
  pose3d.covariance = Eigen::Matrix6d::Zero();
  pose3d.covariance.diagonal() << 1, 2, 3, 0.01, 0.02, 0.03;
  pose3d.covariance(0, 1) = 0.1;
  pose3d.covariance(1, 0) = 0.1;
  pose3d.covariance(3, 5) = 0.002;
  pose3d.covariance(5, 3) = 0.002;

  Eigen::Isometry3d isometry = Eigen::Isometry3d::Identity();
  isometry.translation() << 4, 5, 6;
  isometry.linear() = romea::core::eulerAnglesToRotation3D(Eigen::Vector3d(0.1, 0.2, -0.1));

  romea::core::Pose3D transformed = isometry * pose3d;
  Eigen::Matrix6d numericalJacobian = computeNumericalJacobian(isometry, pose3d);
  Eigen::Matrix6d expectedCovariance =
    numericalJacobian * pose3d.covariance * numericalJacobian.transpose();

  EXPECT_TRUE(transformed.covariance.isApprox(expectedCovariance, 1e-5));
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
