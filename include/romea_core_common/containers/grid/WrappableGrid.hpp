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


#ifndef ROMEA_CORE_COMMON__CONTAINERS__GRID__WRAPPABLEGRID_HPP_
#define ROMEA_CORE_COMMON__CONTAINERS__GRID__WRAPPABLEGRID_HPP_

#include "romea_core_common/containers/grid/Grid.hpp"

namespace romea
{
namespace core
{

template<typename T, size_t DIM>
class WrappableGrid : public Grid<T, DIM>
{
public:
  using CellIndexes = Eigen::Matrix<size_t, DIM, 1>;
  using CellIndexesOffset = Eigen::Matrix<int, DIM, 1>;

public:
  explicit WrappableGrid(const CellIndexes & numberOfCellAlongAxes);

  virtual ~WrappableGrid() = default;

public:
  T & operator()(const CellIndexes & cellIndexes)override;

  const T & operator()(const CellIndexes & cellIndexes)const override;

  void translate(const CellIndexesOffset & indexOffset, const T & emptyValue = T());

  const CellIndexes & getIndexOffsetAlongAxes();

protected:
  CellIndexes wrapCellIndexes_(const CellIndexes & cellIndexes) const;

  size_t computeCellLinearIndex_(const CellIndexes & CellIndexes) const override;

protected:
  CellIndexes indexOffsetsAlongAxes_;
  CellIndexes numberOfCellsAlongAxesMinusOne_;
};

//-----------------------------------------------------------------------------
template<typename T, size_t DIM>
WrappableGrid<T, DIM>::WrappableGrid(const CellIndexes & numberOfCellAlongAxes)
: Grid<T, DIM>(numberOfCellAlongAxes),
  indexOffsetsAlongAxes_(CellIndexes::Zero()),
  numberOfCellsAlongAxesMinusOne_(this->numberOfCellsAlongAxes_ - CellIndexes::Ones())
{
}

//-----------------------------------------------------------------------------
template<typename T, size_t DIM>
typename WrappableGrid<T, DIM>::CellIndexes
WrappableGrid<T, DIM>::wrapCellIndexes_(const CellIndexes & cellIndexes) const
{
  assert((cellIndexes.array() < this->numberOfCellsAlongAxes_.array()).prod());
  // return (cellIndexes + indexOffsetsAlongAxes_)%numberOfCellsAlongAxesMinusOne_;

  CellIndexes wrappredCellIndexes;

  wrappredCellIndexes[0] = (cellIndexes[0] + indexOffsetsAlongAxes_[0]) %
    this->numberOfCellsAlongAxes_[0];

  wrappredCellIndexes[1] = (cellIndexes[1] + indexOffsetsAlongAxes_[1]) %
    this->numberOfCellsAlongAxes_[1];

  // TODO(Jean) utiliser constexpr if
  if (DIM == 3) {
    wrappredCellIndexes[2] = (cellIndexes[2] + indexOffsetsAlongAxes_[2]) %
      this->numberOfCellsAlongAxes_[2];
  }

  return wrappredCellIndexes;
}

//-----------------------------------------------------------------------------
template<typename T, size_t DIM>
size_t WrappableGrid<T, DIM>::computeCellLinearIndex_(const CellIndexes & cellIndexes) const
{
  return wrapCellIndexes_(cellIndexes).dot(this->indexCoefficients_);
}

//-----------------------------------------------------------------------------
template<typename T, size_t DIM>
T & WrappableGrid<T, DIM>::operator()(const CellIndexes & cellIndexes)
{
  return this->buffer_[computeCellLinearIndex_(cellIndexes)];
}

//-----------------------------------------------------------------------------
template<typename T, size_t DIM>
const T & WrappableGrid<T, DIM>::operator()(const CellIndexes & cellIndexes)const
{
  return this->buffer_[computeCellLinearIndex_(cellIndexes)];
}


//-----------------------------------------------------------------------------
template<typename T, size_t DIM>
const typename WrappableGrid<T, DIM>::CellIndexes &
WrappableGrid<T, DIM>::getIndexOffsetAlongAxes()
{
  return indexOffsetsAlongAxes_;
}

//-----------------------------------------------------------------------------
template<typename T, size_t DIM>
void WrappableGrid<T, DIM>::translate(
  const CellIndexesOffset & indexOffset,
  const T & emptyValue)
{
  // TODO(JEAN) utiliser constexpr if
  if (DIM == 2) {
    CellIndexes cellIndexes;
    size_t & xIndex = cellIndexes[0];
    size_t & yIndex = cellIndexes[1];

    const int & indexOffsetAlongXAxis = indexOffset[0];
    const int & indexOffsetAlongYAxis = indexOffset[1];

    const size_t & numberOfCellsAlongXAxis = this->numberOfCellsAlongAxes_[0];
    const size_t & numberOfCellsAlongYAxis = this->numberOfCellsAlongAxes_[1];

    const size_t & numberOfCellsAlongXAxisMinusOne = numberOfCellsAlongAxesMinusOne_[0];
    const size_t & numberOfCellsAlongYAxisMinusOne = numberOfCellsAlongAxesMinusOne_[1];


    // translation along X
    if (indexOffsetAlongXAxis) {
      for (yIndex = 0; yIndex < numberOfCellsAlongYAxis; yIndex++) {
        xIndex = indexOffsetsAlongAxes_[0];
        for (int xOffset = 0; xOffset < indexOffsetAlongXAxis; xOffset++) {
          this->buffer_[computeCellLinearIndex_(cellIndexes)] = emptyValue;
          xIndex = (xIndex + 1) % numberOfCellsAlongXAxis;
        }

        xIndex = indexOffsetsAlongAxes_[0];
        for (int xOffset = 0; xOffset > indexOffsetAlongXAxis; xOffset--) {
          xIndex = (xIndex + numberOfCellsAlongXAxisMinusOne) % numberOfCellsAlongXAxis;
          this->buffer_[computeCellLinearIndex_(cellIndexes)] = emptyValue;
        }
      }
      indexOffsetsAlongAxes_[0] =
        (indexOffsetsAlongAxes_[0] + (numberOfCellsAlongXAxis + indexOffsetAlongXAxis) %
        numberOfCellsAlongXAxis);
    }

    // translation along Y
    if (indexOffsetAlongYAxis) {
      yIndex = indexOffsetsAlongAxes_[1];
      for (int yOffset = 0; yOffset < indexOffsetAlongYAxis; yOffset++) {
        for (xIndex = 0; xIndex < numberOfCellsAlongXAxis; xIndex++) {
          this->buffer_[computeCellLinearIndex_(cellIndexes)] = emptyValue;
        }
        yIndex = (yIndex + 1) % numberOfCellsAlongYAxis;
      }

      for (int yOffset = 0; yOffset > indexOffsetAlongYAxis; yOffset--) {
        yIndex = (yIndex + numberOfCellsAlongYAxisMinusOne) % numberOfCellsAlongYAxis;
        for (xIndex = 0; xIndex < numberOfCellsAlongXAxis; xIndex++) {
          this->buffer_[computeCellLinearIndex_(cellIndexes)] = emptyValue;
        }
      }

      indexOffsetsAlongAxes_[1] =
        (indexOffsetsAlongAxes_[1] + (numberOfCellsAlongYAxis + indexOffsetAlongYAxis)) %
        numberOfCellsAlongYAxis;
    }
  } else {
    CellIndexes cellIndexes;
    size_t & xIndex = cellIndexes[0];
    size_t & yIndex = cellIndexes[1];
    size_t & zIndex = cellIndexes[2];

    const int & indexOffsetAlongXAxis = indexOffset[0];
    const int & indexOffsetAlongYAxis = indexOffset[1];
    const int & indexOffsetAlongZAxis = indexOffset[2];

    const size_t & numberOfCellsAlongXAxis = this->numberOfCellsAlongAxes_[0];
    const size_t & numberOfCellsAlongYAxis = this->numberOfCellsAlongAxes_[1];
    const size_t & numberOfCellsAlongZAxis = this->numberOfCellsAlongAxes_[2];

    const size_t & numberOfCellsAlongXAxisMinusOne = numberOfCellsAlongAxesMinusOne_[0];
    const size_t & numberOfCellsAlongYAxisMinusOne = numberOfCellsAlongAxesMinusOne_[1];
    const size_t & numberOfCellsAlongZAxisMinusOne = numberOfCellsAlongAxesMinusOne_[2];

    // translation along X
    if (indexOffsetAlongXAxis) {
      for (zIndex = 0; zIndex < numberOfCellsAlongZAxis; zIndex++) {
        for (yIndex = 0; yIndex < numberOfCellsAlongYAxis; yIndex++) {
          xIndex = indexOffsetsAlongAxes_[0];
          for (int xOffset = 0; xOffset < indexOffsetAlongXAxis; xOffset++) {
            this->buffer_[computeCellLinearIndex_(cellIndexes)] = emptyValue;
            xIndex = (xIndex + 1) % numberOfCellsAlongXAxis;
          }

          for (int xOffset = 0; xOffset > indexOffsetAlongXAxis; xOffset--) {
            xIndex = (xIndex + numberOfCellsAlongXAxisMinusOne) % numberOfCellsAlongXAxis;
            this->buffer_[computeCellLinearIndex_(cellIndexes)] = emptyValue;
          }
        }
      }
      indexOffsetsAlongAxes_[0] += (numberOfCellsAlongXAxis + indexOffsetAlongXAxis) %
        numberOfCellsAlongXAxis;
    }

    // translation along Y
    if (indexOffsetAlongYAxis) {
      for (zIndex = 0; zIndex < numberOfCellsAlongZAxis; zIndex++) {
        yIndex = indexOffsetsAlongAxes_[1];
        for (int yOffset = 0; yOffset < indexOffsetAlongYAxis; yOffset++) {
          for (xIndex = 0; xIndex < numberOfCellsAlongXAxis; xIndex++) {
            this->buffer_[computeCellLinearIndex_(cellIndexes)] = emptyValue;
          }
          yIndex = (yIndex + 1) % numberOfCellsAlongYAxis;
        }


        for (int yOffset = 0; yOffset > indexOffsetAlongYAxis; yOffset--) {
          yIndex = (yIndex + numberOfCellsAlongYAxisMinusOne) % numberOfCellsAlongYAxis;
          for (xIndex = 0; xIndex < numberOfCellsAlongXAxis; xIndex++) {
            this->buffer_[computeCellLinearIndex_(cellIndexes)] = emptyValue;
          }
        }
      }
      indexOffsetsAlongAxes_[1] += (numberOfCellsAlongYAxis + indexOffsetAlongYAxis) %
        numberOfCellsAlongYAxis;
    }

    // translation along Z
    if (indexOffsetAlongZAxis) {
      zIndex = indexOffsetsAlongAxes_[2];

      for (int zOffset = 0; zOffset < indexOffsetAlongZAxis; zOffset++) {
        for (yIndex = 0; yIndex < numberOfCellsAlongYAxis; yIndex++) {
          for (xIndex = 0; xIndex < numberOfCellsAlongXAxis; xIndex++) {
            this->buffer_[computeCellLinearIndex_(cellIndexes)] = emptyValue;
          }
        }
        zIndex = (zIndex + 1) % numberOfCellsAlongZAxis;
      }

      for (int zOffset = 0; zOffset < indexOffsetAlongZAxis; zOffset++) {
        zIndex = (zIndex + numberOfCellsAlongZAxisMinusOne) % numberOfCellsAlongZAxis;

        for (yIndex = 0; yIndex < numberOfCellsAlongYAxis; yIndex++) {
          for (xIndex = 0; xIndex < numberOfCellsAlongXAxis; xIndex++) {
            this->buffer_[computeCellLinearIndex_(cellIndexes)] = emptyValue;
          }
        }
      }
      indexOffsetsAlongAxes_[2] += (numberOfCellsAlongZAxis + indexOffsetAlongZAxis) %
        numberOfCellsAlongZAxis;
    }
  }
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__CONTAINERS__GRID__WRAPPABLEGRID_HPP_
