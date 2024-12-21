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


#ifndef ROMEA_CORE_COMMON__MATH__TRANSFORMATION_HPP_
#define ROMEA_CORE_COMMON__MATH__TRANSFORMATION_HPP_

// Eigen
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
template<typename Scalar>
inline Eigen::Transform<Scalar, 3, Eigen::Affine> rigid_transformation3(
  const Eigen::Matrix<Scalar, 3, 1> & translation,
  const Eigen::Matrix<Scalar, 3, 1> & angles)
{
  using EigenVector = Eigen::Matrix<Scalar, 3, 1>;
  using EigenAngleAxis = Eigen::AngleAxis<Scalar>;
  using EigenTransform = Eigen::Transform<Scalar, 3, Eigen::Affine>;

  EigenTransform rigidTransformation = EigenTransform::Identity();
  rigidTransformation.translate(translation);
  rigidTransformation.rotate(EigenAngleAxis(angles[0], EigenVector::UnitX()));
  rigidTransformation.rotate(EigenAngleAxis(angles[1], EigenVector::UnitY()));
  rigidTransformation.rotate(EigenAngleAxis(angles[2], EigenVector::UnitZ()));
  return rigidTransformation;
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__MATH__TRANSFORMATION_HPP_
