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

#ifndef ROMEA_CORE_COMMON__POINTSET__KDTREE__NEIGHBOR_HPP_
#define ROMEA_CORE_COMMON__POINTSET__KDTREE__NEIGHBOR_HPP_

#include <cstdlib>

namespace romea
{
namespace core
{


template<typename ScalarType>
struct Neighbor
{
  size_t pointIndex;
  ScalarType pointSquaredDistance;
  Neighbor(const size_t & pointIndex, const ScalarType & pointSquareDistance);
};


//-----------------------------------------------------------------------------
template<typename ScalarType>
Neighbor<ScalarType>::Neighbor(
  const size_t & pointIndex,
  const ScalarType & pointSquaredDistance)
: pointIndex(pointIndex), pointSquaredDistance(pointSquaredDistance)
{
}


//-----------------------------------------------------------------------------
template<typename ScalarType>
inline bool operator<(
  const Neighbor<ScalarType> & firstEntry,
  const Neighbor<ScalarType> & secondEntry)
{
  return firstEntry.pointSquaredDistance < secondEntry.pointSquaredDistance;
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__POINTSET__KDTREE__NEIGHBOR_HPP_
