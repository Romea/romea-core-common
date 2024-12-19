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


#ifndef ROMEA_CORE_COMMON__MATH__ALGORITHM_HPP_
#define ROMEA_CORE_COMMON__MATH__ALGORITHM_HPP_

// std
#include <optional>
#include <cmath>
#include <cassert>
#include <limits>
#include <algorithm>
#include <iostream>
#include <sstream>

namespace romea
{
namespace core
{

//--------------------------------------------------------------------------
template<typename RealType>
RealType sign(const RealType & x)
{
  return (x < 0.) ? -1 : 1;
}

//--------------------------------------------------------------------------
template<typename RealType>
double signedMin(const RealType & x, const RealType & y)
{
  if (x >= 0 || y >= 0) {
    return std::min(x, y);
  } else {
    return std::max(x, y);
  }
}

//--------------------------------------------------------------------------
template<typename RealType>
double signedFloor(const RealType & x)
{
  if (x >= 0) {
    return std::floor(x);
  } else {
    return std::ceil(x);
  }
}

//--------------------------------------------------------------------------
template<typename RealType>
RealType near(
  const RealType & value1,
  const RealType & value2,
  double epsilon = std::numeric_limits<RealType>::epsilon())
{
  return std::abs(value1 - value2) < 0.00001;
}

// replace by std::clamp with c++17
//--------------------------------------------------------------------------
template<typename RealType>
RealType clamp(
  const RealType & value,
  const RealType & minValue,
  const RealType & maxValue)
{
  assert(minValue <= maxValue);

  if (std::isnan(value)) {
    std::stringstream msg;
    msg << "Cannot clamp Nan value between ";
    msg << minValue;
    msg << " and ";
    msg << maxValue;
    throw std::runtime_error(msg.str());
  }

  return std::max(minValue, std::min(maxValue, value));
}

//--------------------------------------------------------------------------
template<typename RealType>
RealType symmetricClamp(
  const RealType & value,
  const RealType & absoluteMaxValue)
{
  return clamp(value, -absoluteMaxValue, absoluteMaxValue);
}

//--------------------------------------------------------------------------
template<typename RealType>
std::optional<RealType> safeDivide(
  const RealType & numerator,
  const RealType & denominator)
{
  if (std::abs(denominator) < std::numeric_limits<RealType>::epsilon()) {
    return std::optional<RealType>();
  } else {
    return std::optional<RealType>(numerator / denominator);
  }
}

//--------------------------------------------------------------------------
template<typename RealType>
static bool isApproximatelyEqual(
  RealType a,
  RealType b,
  RealType tolerance = std::numeric_limits<RealType>::epsilon())
{
  RealType diff = std::fabs(a - b);
  if (diff <= tolerance) {
    return true;
  }

  if (diff < std::fmax(std::fabs(a), std::fabs(b)) * tolerance) {
    return true;
  }

  return false;
}

//--------------------------------------------------------------------------
template<typename RealType>
static bool isApproximatelyZero(
  RealType a,
  RealType tolerance = std::numeric_limits<RealType>::epsilon())
{
  if (std::fabs(a) <= tolerance) {
    return true;
  }
  return false;
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__MATH__ALGORITHM_HPP_
