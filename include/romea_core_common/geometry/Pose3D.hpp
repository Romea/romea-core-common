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

#ifndef ROMEA_CORE_COMMON__GEOMETRY__POSE3D_HPP_
#define ROMEA_CORE_COMMON__GEOMETRY__POSE3D_HPP_

// Eigen
#include <Eigen/Geometry>

// romea
#include "romea_core_common/geometry/Pose2D.hpp"
#include "romea_core_common/geometry/Position3D.hpp"
#include "romea_core_common/math/Matrix.hpp"

namespace romea
{
namespace core
{

struct Pose3D
{
  Pose3D();

  Eigen::Vector3d position;
  Eigen::Vector3d orientation;
  Eigen::Matrix6d covariance;
};

Pose2D toPose2D(const Pose3D & pose3d);

Position3D toPosition3D(const Pose3D & pose3d);

void toPose2D(const Pose3D & pose3d, Pose2D & pose2d);

void toPosition3D(const Pose3D & pose3d, Position3D & position3d);

std::ostream & operator<<(std::ostream & os, const Pose3D & pose3D);

Pose3D operator*(const Eigen::Affine3d & affine, const Pose3D & pose3d);

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__GEOMETRY__POSE3D_HPP_
