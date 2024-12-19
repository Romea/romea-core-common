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


#ifndef ROMEA_CORE_COMMON__TRANSFORM__ESTIMATION__RIGIDTRANSFORMATIONPROJECTION_HPP_
#define ROMEA_CORE_COMMON__TRANSFORM__ESTIMATION__RIGIDTRANSFORMATIONPROJECTION_HPP_

#include <romea_core_common/coordinates/CartesianCoordinates.hpp>
#include <romea_core_common/coordinates/HomogeneousCoordinates.hpp>

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
template<typename Scalar>
void projection(
  const Eigen::Matrix<Scalar, 3, 3> & tranformationMatrix,
  const CartesianCoordinates2<Scalar> & sourcePoint,
  CartesianCoordinates2<Scalar> & targetPoint)
{
  targetPoint = tranformationMatrix.template block<2, 2>(0, 0) * sourcePoint +
    tranformationMatrix.template block<2, 1>(0, 2);
}

//-----------------------------------------------------------------------------
template<typename Scalar>
void projection(
  const Eigen::Matrix<Scalar, 4, 4> & tranformationMatrix,
  const CartesianCoordinates3<Scalar> & sourcePoint,
  CartesianCoordinates3<Scalar> & targetPoint)
{
  targetPoint = tranformationMatrix.template block<3, 3>(0, 0) * sourcePoint +
    tranformationMatrix.template block<3, 1>(0, 3);
}

//-----------------------------------------------------------------------------
template<typename Scalar>
void projection(
  const Eigen::Matrix<Scalar, 3, 3> & tranformationMatrix,
  const HomogeneousCoordinates2<Scalar> & sourcePoint,
  HomogeneousCoordinates2<Scalar> & targetPoint)
{
  targetPoint = tranformationMatrix * sourcePoint;
}

//-----------------------------------------------------------------------------
template<typename Scalar>
void projection(
  const Eigen::Matrix<Scalar, 4, 4> & tranformationMatrix,
  const HomogeneousCoordinates3<Scalar> & sourcePoint,
  HomogeneousCoordinates3<Scalar> & targetPoint)
{
  targetPoint = tranformationMatrix * sourcePoint;
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__TRANSFORM__ESTIMATION__RIGIDTRANSFORMATIONPROJECTION_HPP_
