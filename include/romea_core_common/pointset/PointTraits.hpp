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


#ifndef ROMEA_CORE_COMMON__POINTSET__POINTTRAITS_HPP_
#define ROMEA_CORE_COMMON__POINTSET__POINTTRAITS_HPP_

#include "romea_core_common/coordinates/CartesianCoordinates.hpp"
#include "romea_core_common/coordinates/HomogeneousCoordinates.hpp"

namespace romea
{
namespace core
{


template<class PointType>
struct PointTraits
{
  static constexpr int DIM = 0;
  static constexpr int SIZE = 0;
};

template<>
struct PointTraits<CartesianCoordinates2f>
{
  static constexpr int DIM = 2;
  static constexpr int SIZE = 2;
};

template<>
struct PointTraits<CartesianCoordinates2d>
{
  static constexpr int DIM = 2;
  static constexpr int SIZE = 2;
};

template<>
struct PointTraits<CartesianCoordinates3f>
{
  static constexpr int DIM = 3;
  static constexpr int SIZE = 3;
};

template<>
struct PointTraits<CartesianCoordinates3d>
{
  static constexpr int DIM = 3;
  static constexpr int SIZE = 3;
};

template<>
struct PointTraits<HomogeneousCoordinates2f>
{
  static constexpr int DIM = 2;
  static constexpr int SIZE = 3;
};

template<>
struct PointTraits<HomogeneousCoordinates2d>
{
  static constexpr int DIM = 2;
  static constexpr int SIZE = 3;
};

template<>
struct PointTraits<HomogeneousCoordinates3f>
{
  static constexpr int DIM = 3;
  static constexpr int SIZE = 4;
};

template<>
struct PointTraits<Eigen::Vector4f>
{
  static constexpr int DIM = 3;
  static constexpr int SIZE = 4;
};

template<>
struct PointTraits<HomogeneousCoordinates3d>
{
  static constexpr int DIM = 3;
  static constexpr int SIZE = 4;
};

template<>
struct PointTraits<Eigen::Vector4d>
{
  static constexpr int DIM = 3;
  static constexpr int SIZE = 4;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__POINTSET__POINTTRAITS_HPP_
