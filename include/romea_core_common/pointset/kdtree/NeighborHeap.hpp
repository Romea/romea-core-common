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

#ifndef ROMEA_CORE_COMMON__POINTSET__KDTREE__NEIGHBORHEAP_HPP_
#define ROMEA_CORE_COMMON__POINTSET__KDTREE__NEIGHBORHEAP_HPP_

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
class NeighborHeap
{
private:
  typedef  std::vector<Neighbor<ScalarType>> Neighbors;

public:
  explicit NeighborHeap(const size_t & capacity);

  const ScalarType & getWorstSquareDistance() const;

  const Neighbors & getNeighbors()const;

  size_t getSize()const;

  void insert(const size_t & pointIndex, const ScalarType & pointSquareDistance);
  void reset();
  void sort();

private:
  Neighbors neighbors_;
  const size_t heapCapacity_;
};


//-----------------------------------------------------------------------------
template<typename ScalarType>
NeighborHeap<ScalarType>::NeighborHeap(const size_t & capacity)
: neighbors_(1, Neighbor<ScalarType>(0, std::numeric_limits<ScalarType>::infinity())),
  heapCapacity_(capacity)
{
  neighbors_.reserve(heapCapacity_);
}

//-----------------------------------------------------------------------------
template<typename ScalarType>
void
NeighborHeap<ScalarType>::reset()
{
  neighbors_.clear();
  neighbors_.emplace_back(Neighbor<ScalarType>(0, std::numeric_limits<ScalarType>::infinity()));
}

//-----------------------------------------------------------------------------
template<typename ScalarType>
const ScalarType &
NeighborHeap<ScalarType>::getWorstSquareDistance() const
{
  return neighbors_.front().pointSquaredDistance;
}

//-----------------------------------------------------------------------------
template<typename ScalarType>
const typename NeighborHeap<ScalarType>::Neighbors &
NeighborHeap<ScalarType>::getNeighbors()const
{
  return neighbors_;
}

//-----------------------------------------------------------------------------
template<typename ScalarType>
size_t
NeighborHeap<ScalarType>::getSize()const
{
  return neighbors_.size();
}


//-----------------------------------------------------------------------------
template<typename ScalarType>
void
NeighborHeap<ScalarType>::insert(
  const size_t & pointIndex,
  const ScalarType & pointSquareDistance)
{
  if (neighbors_.size() == heapCapacity_) {
    std::pop_heap(neighbors_.begin(), neighbors_.end());
    neighbors_.back() = std::move(Neighbor<ScalarType>(pointIndex, pointSquareDistance));
  } else {
    neighbors_.emplace_back(Neighbor<ScalarType>(pointIndex, pointSquareDistance));
  }

  // ensure heap
  std::push_heap(neighbors_.begin(), neighbors_.end());
}

//-----------------------------------------------------------------------------
template<typename ScalarType>
void
NeighborHeap<ScalarType>::sort()
{
  std::sort_heap(neighbors_.begin(), neighbors_.end());
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__POINTSET__KDTREE__NEIGHBORHEAP_HPP_
