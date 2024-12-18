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

#ifndef ROMEA_CORE_COMMON__POINTSET__KDTREE__NEIGHBORVECTOR_HPP_
#define ROMEA_CORE_COMMON__POINTSET__KDTREE__NEIGHBORVECTOR_HPP_

// std
#include <vector>
#include <algorithm>
#include <limits>
#include <utility>

// romea
#include "Neighbor.hpp"

namespace romea
{
namespace core
{


template<typename ScalarType>
class NeighborVector
{
private:
  typedef  std::vector<Neighbor<ScalarType>> Neighbors;

public:
  explicit NeighborVector(const size_t & size);

  const ScalarType & getWorstSquareDistance() const;

  const Neighbors & getNeighbors()const;

  size_t getSize()const;

  void insert(const size_t & pointIndex, const ScalarType & pointSquareDistance);
  void reset();
  void sort();

private:
  Neighbors neighbors_;
  const size_t vectorSizeMinusOne_;
  const ScalarType & worstSquareDistanceRef_;
};


//-----------------------------------------------------------------------------
template<typename ScalarType>
NeighborVector<ScalarType>::NeighborVector(const size_t & size)
: neighbors_(size, Neighbor<ScalarType>(0, std::numeric_limits<ScalarType>::infinity())),
  vectorSizeMinusOne_(size - 1),
  worstSquareDistanceRef_(neighbors_.back().pointSquaredDistance)
{
}

//-----------------------------------------------------------------------------
template<typename ScalarType>
void
NeighborVector<ScalarType>::reset()
{
  for (auto & neighbor : neighbors_) {
    neighbor.pointSquaredDistance = std::numeric_limits<ScalarType>::infinity();
    neighbor.pointIndex = 0;
  }
}

//-----------------------------------------------------------------------------
template<typename ScalarType>
const typename NeighborVector<ScalarType>::Neighbors &
NeighborVector<ScalarType>::getNeighbors()const
{
  return neighbors_;
}

//-----------------------------------------------------------------------------
template<typename ScalarType>
size_t
NeighborVector<ScalarType>::getSize()const
{
  return neighbors_.size();
}

//-----------------------------------------------------------------------------
template<typename ScalarType>
void
NeighborVector<ScalarType>::insert(
  const size_t & pointIndex,
  const ScalarType & pointSquaredDistance)
{
  size_t i;

  //    for (i = vectorSizeMinusOne_; i > 0; --i)
  //    {
  //      if (data[i-1].pointSquareDistance > pointSquareDistance)
  //        entries[i] = entries[i-1];
  //      else
  //        break;
  //    }

  for (i = vectorSizeMinusOne_; i > 0; --i) {
    if (neighbors_[i - 1].pointSquaredDistance > pointSquaredDistance) {
      std::swap(neighbors_[i], neighbors_[i - 1]);
    } else {
      break;
    }
  }

  neighbors_[i].pointIndex = pointIndex;
  neighbors_[i].pointSquaredDistance = pointSquaredDistance;
}

//-----------------------------------------------------------------------------
template<typename ScalarType>
const ScalarType &
NeighborVector<ScalarType>::getWorstSquareDistance() const
{
  return worstSquareDistanceRef_;
}

//-----------------------------------------------------------------------------
template<typename ScalarType>
void
NeighborVector<ScalarType>::sort()
{
  // no need to sort as entries are already sorted
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__POINTSET__KDTREE__NEIGHBORVECTOR_HPP_
