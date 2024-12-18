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


#ifndef ROMEA_CORE_COMMON__CONTAINERS__GRID__GRIDINDEXMAPPING_HPP_
#define ROMEA_CORE_COMMON__CONTAINERS__GRID__GRIDINDEXMAPPING_HPP_

// Eigen
#include <Eigen/Core>

// std
#include <cstddef>
#include <vector>

// Romea
#include "romea_core_common/math/Interval.hpp"

namespace romea
{
namespace core
{

template<typename Scalar, size_t DIM>
class GridIndexMapping
{
public:
  using IntervalType = Interval<Scalar, DIM>;
  using PointType = Eigen::Matrix<Scalar, DIM, 1>;
  using CellIndexes = Eigen::Matrix<size_t, DIM, 1>;
  using CellIndexesOffset = Eigen::Matrix<int, DIM, 1>;

public:
  GridIndexMapping();


  GridIndexMapping(
    const Scalar & maximalRange,
    const Scalar & cellResolution);

  GridIndexMapping(
    const PointType & center,
    const Scalar & maximalRange,
    const Scalar & cellResolution);

  GridIndexMapping(
    const IntervalType & extremities,
    const Scalar & cellResolution);

public:
  const Scalar & getCellResolution()const;

  const CellIndexes & getNumberOfCellsAlongAxes()const;

  const std::vector<Scalar> & getCellCentersPositionAlong(const size_t & axisDIM) const;

  CellIndexesOffset updateCenter(const PointType & center);

public:
  CellIndexes computeCellIndexes(const PointType & point) const;

  PointType computeCellCenterPosition(const CellIndexes & cellIndexes) const;

protected:
  void computeCellCentersPosition_();

protected:
  Scalar cellResolution_;
  CellIndexes numberOfCellsAlongAxes_;
  PointType minimalValuesAlongAxes_;
  PointType maximalValuesAlongAxes_;
  std::vector<std::vector<Scalar>> cellCentersPositionAlongAxes_;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF_VECTORIZABLE_FIXED_SIZE(Scalar, DIM)
};

using GridIndexMapping2f = GridIndexMapping<float, 2>;
using GridIndexMapping3f = GridIndexMapping<float, 3>;
using GridIndexMapping2d = GridIndexMapping<double, 2>;
using GridIndexMapping3d = GridIndexMapping<double, 3>;

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__CONTAINERS__GRID__GRIDINDEXMAPPING_HPP_
