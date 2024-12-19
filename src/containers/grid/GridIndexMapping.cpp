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


// std
#include <vector>
#include <iostream>

// local
#include "romea_core_common/containers/grid/GridIndexMapping.hpp"

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
template<typename Scalar, size_t DIM>
GridIndexMapping<Scalar, DIM>::GridIndexMapping()
: cellResolution_(0.),
  numberOfCellsAlongAxes_(),
  minimalValuesAlongAxes_(),
  maximalValuesAlongAxes_(),
  cellCentersPositionAlongAxes_(DIM)
{
}

//-----------------------------------------------------------------------------
template<typename Scalar, size_t DIM>
GridIndexMapping<Scalar, DIM>::GridIndexMapping(
  const IntervalType & extrimities,
  const Scalar & cellResolution)
: cellResolution_(cellResolution),
  numberOfCellsAlongAxes_(),
  minimalValuesAlongAxes_(),
  maximalValuesAlongAxes_(),
  cellCentersPositionAlongAxes_(DIM)
{
  minimalValuesAlongAxes_.array() = cellResolution_ *
    (round((extrimities.lower().array() / cellResolution_)) - 0.5);

  maximalValuesAlongAxes_.array() = cellResolution_ *
    (round((extrimities.upper().array() / cellResolution_)) + 0.5);

  numberOfCellsAlongAxes_.array() = (round(extrimities.upper().array() / cellResolution_) -
    round(extrimities.lower().array() / cellResolution_) + 1).template cast<size_t>();

  computeCellCentersPosition_();
}

//-----------------------------------------------------------------------------
template<typename Scalar, size_t DIM>
GridIndexMapping<Scalar, DIM>::GridIndexMapping(
  const Scalar & maximalRange,
  const Scalar & cellResolution)
: GridIndexMapping(IntervalType(PointType::Constant(-maximalRange),
    PointType::Constant(maximalRange)), cellResolution)
{
}

//-----------------------------------------------------------------------------
template<typename Scalar, size_t DIM>
GridIndexMapping<Scalar, DIM>::GridIndexMapping(
  const PointType & center,
  const Scalar & maximalRange,
  const Scalar & cellResolution)
: GridIndexMapping(
    IntervalType(
      center + PointType::Constant(-maximalRange),
      center + PointType::Constant(maximalRange)), cellResolution)
{
}


//-----------------------------------------------------------------------------
template<typename Scalar, size_t DIM>
void GridIndexMapping<Scalar, DIM>::computeCellCentersPosition_()
{
  for (size_t dim = 0; dim < DIM; dim++) {
    const size_t & numberOfCellsAlongAxis = numberOfCellsAlongAxes_[dim];
    const Scalar & minimalValueAlongAxis = minimalValuesAlongAxes_[dim];
    std::vector<Scalar> & cellCentersAlongAxis = cellCentersPositionAlongAxes_[dim];

    cellCentersAlongAxis.resize(numberOfCellsAlongAxis);
    for (size_t n = 0; n < numberOfCellsAlongAxis; ++n) {
      cellCentersAlongAxis[n] = minimalValueAlongAxis + (n + Scalar(0.5)) * cellResolution_;
    }
  }
}

//-----------------------------------------------------------------------------
template<typename Scalar, size_t DIM>
typename GridIndexMapping<Scalar, DIM>::CellIndexesOffset
GridIndexMapping<Scalar, DIM>::updateCenter(const PointType & center)
{
  PointType currentRoundedCenter = computeCellCenterPosition(numberOfCellsAlongAxes_ / 2);
  PointType newRoundedCenter = cellResolution_ * (round((center.array() / cellResolution_)));

  minimalValuesAlongAxes_ = newRoundedCenter -
    numberOfCellsAlongAxes_.template cast<Scalar>() / 2 * cellResolution_;
  maximalValuesAlongAxes_ = newRoundedCenter +
    numberOfCellsAlongAxes_.template cast<Scalar>() / 2 * cellResolution_;

  computeCellCentersPosition_();

  return ((newRoundedCenter - currentRoundedCenter) / cellResolution_).template cast<int>();
}

//-----------------------------------------------------------------------------
template<typename Scalar, size_t DIM>
const Scalar & GridIndexMapping<Scalar, DIM>::getCellResolution()const
{
  return cellResolution_;
}

//-----------------------------------------------------------------------------
template<typename Scalar, size_t DIM>
const typename GridIndexMapping<Scalar, DIM>::CellIndexes &
GridIndexMapping<Scalar, DIM>::getNumberOfCellsAlongAxes()const
{
  return numberOfCellsAlongAxes_;
}

//-----------------------------------------------------------------------------
template<typename Scalar, size_t DIM>
typename GridIndexMapping<Scalar, DIM>::CellIndexes
GridIndexMapping<Scalar, DIM>::computeCellIndexes(const PointType & point)const
{
  return ((point - minimalValuesAlongAxes_) / cellResolution_).template cast<size_t>();
}

//-----------------------------------------------------------------------------
template<typename Scalar, size_t DIM>
typename GridIndexMapping<Scalar, DIM>::PointType
GridIndexMapping<Scalar, DIM>::computeCellCenterPosition(const CellIndexes & cellIndexes) const
{
  PointType point;
  for (size_t dim = 0; dim < DIM; dim++) {
    point(dim) = cellCentersPositionAlongAxes_[dim][cellIndexes(dim)];
  }

  return point;
}

//-----------------------------------------------------------------------------
template<typename Scalar, size_t DIM>
const std::vector<Scalar> &
GridIndexMapping<Scalar, DIM>::getCellCentersPositionAlong(const size_t & axisDIM) const
{
  return cellCentersPositionAlongAxes_[axisDIM];
}

template class GridIndexMapping<float, 2>;
template class GridIndexMapping<float, 3>;
template class GridIndexMapping<double, 2>;
template class GridIndexMapping<double, 3>;

}  // namespace core
}  // namespace romea
