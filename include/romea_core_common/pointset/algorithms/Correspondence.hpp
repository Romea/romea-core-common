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


#ifndef ROMEA_CORE_COMMON__POINTSET__ALGORITHMS__CORRESPONDENCE_HPP_
#define ROMEA_CORE_COMMON__POINTSET__ALGORITHMS__CORRESPONDENCE_HPP_

// std
#include <cstddef>

namespace romea
{
namespace core
{

struct Correspondence
{
  Correspondence();

  Correspondence(
    const size_t & sourcePointIndex,
    const size_t & targetPointIndex);

  Correspondence(
    const size_t & sourcePointIndex,
    const size_t & targetPointIndex,
    const double & squareDistanceBetweenPoints);

  Correspondence(
    const size_t & sourcePointIndex,
    const size_t & targetPointIndex,
    const double & squareDistanceBetweenPoints,
    const double & weight);

  size_t sourcePointIndex;

  size_t targetPointIndex;

  double squareDistanceBetweenPoints;

  double weight;
};


//-----------------------------------------------------------------------------
inline bool sortByTargetIndexAndDistancePredicate(
  const Correspondence & c1,
  const Correspondence & c2)
{
  if (c1.targetPointIndex < c2.targetPointIndex) {
    return true;
  } else if ((c1.targetPointIndex == c2.targetPointIndex) &&
    (c1.squareDistanceBetweenPoints < c2.squareDistanceBetweenPoints))
  {
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------
inline bool sortBySourceIndexAndDistancePredicate(
  const Correspondence & c1,
  const Correspondence & c2)
{
  if (c1.sourcePointIndex < c2.sourcePointIndex) {
    return true;
  } else if ((c1.sourcePointIndex == c2.sourcePointIndex) &&
    (c1.squareDistanceBetweenPoints < c2.squareDistanceBetweenPoints))
  {
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------
inline bool equalTargetIndexesPredicate(
  const Correspondence & c1,
  const Correspondence & c2)
{
  return c1.targetPointIndex == c2.targetPointIndex;
}

//-----------------------------------------------------------------------------
inline bool equalSourceIndexesPredicate(
  const Correspondence & c1,
  const Correspondence & c2)
{
  return c1.sourcePointIndex == c2.sourcePointIndex;
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__POINTSET__ALGORITHMS__CORRESPONDENCE_HPP_
