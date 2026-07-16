// Copyright 2022 INRAE, French National Research Institute for Agriculture,
// Food and Environment
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

#ifndef GEOMETRY__TEST_GEOMETRY_UTILS_HPP_
#define GEOMETRY__TEST_GEOMETRY_UTILS_HPP_

//-----------------------------------------------------------------------------
template <typename MsgCovType>
void fillMsgCovariance(MsgCovType& covariance, size_t start = 0) {
  for (size_t n = 0; n < covariance.size(); ++n) {
    covariance[n] = n + start;
  }
}

//-----------------------------------------------------------------------------
template <typename EigenVectorType>
void fillEigenVector(EigenVectorType& vector, int start = 0) {
  for (int n = 0; n < vector.rows(); ++n) {
    vector(n) = n + start;
  }
}

//-----------------------------------------------------------------------------
template <typename EigenCovType>
void fillEigenCovariance(EigenCovType& covariance, int start = 0) {
  for (int n = 0; n < covariance.rows() * covariance.cols(); ++n) {
    covariance(n) = n + start;
  }
}

//-----------------------------------------------------------------------------
template <typename MsgCovType, typename EigenCovType>
void isSame(const MsgCovType& msgCovariance,
            const EigenCovType& obsCovariance) {
  for (size_t n = 0; n < msgCovariance.size(); ++n) {
    EXPECT_DOUBLE_EQ(msgCovariance[n], obsCovariance(n));
  }
}

//-----------------------------------------------------------------------------
template <typename EigenCovType>
void isSame(const EigenCovType& obsCovariance1,
            const EigenCovType& obsCovariance2) {
  for (int n = 0; n < obsCovariance1.cols() * obsCovariance1.rows(); ++n) {
    EXPECT_DOUBLE_EQ(obsCovariance1(n), obsCovariance2(n));
  }
}

//-----------------------------------------------------------------------------
template <typename Se2CovarianceType, typename Se3CovarianceType>
void isSameSe2Covariance(const Se2CovarianceType& se2Covariance,
                         const Se3CovarianceType& se3Covariance) {
  EXPECT_DOUBLE_EQ(se2Covariance(0, 0), se3Covariance(0, 0));
  EXPECT_DOUBLE_EQ(se2Covariance(0, 1), se3Covariance(0, 1));
  EXPECT_DOUBLE_EQ(se2Covariance(0, 2), se3Covariance(0, 5));
  EXPECT_DOUBLE_EQ(se2Covariance(1, 0), se3Covariance(1, 0));
  EXPECT_DOUBLE_EQ(se2Covariance(1, 1), se3Covariance(1, 1));
  EXPECT_DOUBLE_EQ(se2Covariance(1, 2), se3Covariance(1, 5));
  EXPECT_DOUBLE_EQ(se2Covariance(2, 0), se3Covariance(5, 0));
  EXPECT_DOUBLE_EQ(se2Covariance(2, 1), se3Covariance(5, 1));
  EXPECT_DOUBLE_EQ(se2Covariance(2, 2), se3Covariance(5, 5));
}

//-----------------------------------------------------------------------------
template <typename Pose2DType, typename Pose3DType>
void isSamePose2D(const Pose2DType& pose2d,
                  const Pose3DType& pose3d) {
  EXPECT_DOUBLE_EQ(pose2d.yaw, pose3d.orientation.z());
  EXPECT_DOUBLE_EQ(pose2d.position.x(), pose3d.position.x());
  EXPECT_DOUBLE_EQ(pose2d.position.y(), pose3d.position.y());
  isSameSe2Covariance(pose2d.covariance, pose3d.covariance);
}

//-----------------------------------------------------------------------------
template <typename Twist2DType, typename Twist3DType>
void isSameTwist2D(const Twist2DType& twist2d,
                   const Twist3DType& twist3d) {
  EXPECT_DOUBLE_EQ(twist2d.linearSpeeds.x(), twist3d.linearSpeeds.x());
  EXPECT_DOUBLE_EQ(twist2d.linearSpeeds.y(), twist3d.linearSpeeds.y());
  EXPECT_DOUBLE_EQ(twist2d.angularSpeed, twist3d.angularSpeeds.z());
  isSameSe2Covariance(twist2d.covariance, twist3d.covariance);
}

//-----------------------------------------------------------------------------
template <typename Position3DType, typename Pose3DType>
void isSamePosition3D(const Position3DType& position3d,
                      const Pose3DType& pose3d) {
  EXPECT_DOUBLE_EQ(position3d.position.x(), pose3d.position.x());
  EXPECT_DOUBLE_EQ(position3d.position.y(), pose3d.position.y());
  EXPECT_DOUBLE_EQ(position3d.position.z(), pose3d.position.z());
  isSame(position3d.covariance, Eigen::Matrix3d(pose3d.covariance.template block<3, 3>(0, 0)));
}

#endif  // GEOMETRY__TEST_GEOMETRY_UTILS_HPP_
