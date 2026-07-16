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


// romea
#include "romea_core_common/transform/SmartRotation3D.hpp"

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
SmartRotation3D::SmartRotation3D()
: Rx_(Eigen::Matrix3d::Identity()),
  Ry_(Eigen::Matrix3d::Identity()),
  Rz_(Eigen::Matrix3d::Identity()),
  R_(Eigen::Matrix3d::Identity()),
  dRxdAngleX_(Eigen::Matrix3d::Identity()),
  dRydAngleY_(Eigen::Matrix3d::Identity()),
  dRzdAngleZ_(Eigen::Matrix3d::Identity()),
  dRdAngleX_(Eigen::Matrix3d::Zero()),
  dRdAngleY_(Eigen::Matrix3d::Zero()),
  dRdAngleZ_(Eigen::Matrix3d::Zero())
{
}

//-----------------------------------------------------------------------------
SmartRotation3D::SmartRotation3D(
  const double & angleAroundXAxis,
  const double & angleAroundYAxis,
  const double & angleAroundZAxis)
: SmartRotation3D()
{
  init(angleAroundXAxis, angleAroundYAxis, angleAroundZAxis);
}

//-----------------------------------------------------------------------------
SmartRotation3D::SmartRotation3D(const Eigen::Vector3d & angles)
: SmartRotation3D(angles[0], angles[1], angles[2])
{
}

//-----------------------------------------------------------------------------
void SmartRotation3D::init(
  const double & angleAroundXAxis,
  const double & angleAroundYAxis,
  const double & angleAroundZAxis)
{
  double cosx = std::cos(angleAroundXAxis);
  double sinx = std::sin(angleAroundXAxis);

  double cosy = std::cos(angleAroundYAxis);
  double siny = std::sin(angleAroundYAxis);

  double cosz = std::cos(angleAroundZAxis);
  double sinz = std::sin(angleAroundZAxis);

  Rx_.setIdentity();
  Ry_.setIdentity();
  Rz_.setIdentity();
  dRxdAngleX_.setZero();
  dRydAngleY_.setZero();
  dRzdAngleZ_.setZero();

  // Compute Rx
  Rx_(1, 1) = cosx;
  Rx_(1, 2) = -sinx;
  Rx_(2, 1) = sinx;
  Rx_(2, 2) = cosx;

  // Compute Ry
  Ry_(0, 0) = cosy;
  Ry_(0, 2) = siny;
  Ry_(2, 0) = -siny;
  Ry_(2, 2) = cosy;

  // Compute Rz
  Rz_(0, 0) = cosz;
  Rz_(0, 1) = -sinz;
  Rz_(1, 0) = sinz;
  Rz_(1, 1) = cosz;

  // Compute R
  R_ = Rz_ * Ry_ * Rx_;

  // compute derivative of Rx with respect x axis angle
  dRxdAngleX_(1, 1) = -sinx;
  dRxdAngleX_(1, 2) = -cosx;
  dRxdAngleX_(2, 1) = cosx;
  dRxdAngleX_(2, 2) = -sinx;

  // compute derivative of Ry with respect y axis angle
  dRydAngleY_(0, 0) = -siny;
  dRydAngleY_(0, 2) = cosy;
  dRydAngleY_(2, 0) = -cosy;
  dRydAngleY_(2, 2) = -siny;

  // compute derivative of Rz with respect z axis angle
  dRzdAngleZ_(0, 0) = -sinz;
  dRzdAngleZ_(0, 1) = -cosz;
  dRzdAngleZ_(1, 0) = cosz;
  dRzdAngleZ_(1, 1) = -sinz;

  // compute derivative of R with respect x y z axis angles
  dRdAngleX_ = Rz_ * Ry_ * dRxdAngleX_;
  dRdAngleY_ = Rz_ * dRydAngleY_ * Rx_;
  dRdAngleZ_ = dRzdAngleZ_ * Ry_ * Rx_;
}

//-----------------------------------------------------------------------------
void SmartRotation3D::init(const Eigen::Vector3d & angles)
{
  init(angles[0], angles[1], angles[2]);
}

//-----------------------------------------------------------------------------
Eigen::Vector3d SmartRotation3D::operator*(const Eigen::Vector3d & T)const
{
  return R_ * T;
}

//-----------------------------------------------------------------------------
const Eigen::Matrix3d & SmartRotation3D::R() const
{
  return R_;
}

//-----------------------------------------------------------------------------
Eigen::Matrix3d SmartRotation3D::dRTdAngles(const Eigen::Vector3d & T) const
{
  Eigen::Matrix3d dRTdAngles;
  dRTdAngles.col(0) = dRdAngleX_ * T;
  dRTdAngles.col(1) = dRdAngleY_ * T;
  dRTdAngles.col(2) = dRdAngleZ_ * T;
  return dRTdAngles;
}

//-----------------------------------------------------------------------------
const Eigen::Matrix3d & SmartRotation3D::dRdAngleAroundXAxis()const
{
  return dRdAngleX_;
}

//-----------------------------------------------------------------------------
const Eigen::Matrix3d & SmartRotation3D::dRdAngleAroundYAxis()const
{
  return dRdAngleY_;
}

//-----------------------------------------------------------------------------
const Eigen::Matrix3d & SmartRotation3D::dRdAngleAroundZAxis()const
{
  return dRdAngleZ_;
}

}   // namespace core
}   // namespace romea
