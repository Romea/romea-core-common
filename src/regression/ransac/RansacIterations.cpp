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


// romea
#include "romea_core_common/regression/ransac/RansacIterations.hpp"

// stl
#include <cmath>
#include <algorithm>
#include <limits>

namespace
{
const double EPSILON = std::numeric_limits<double>::epsilon();
}

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
RansacIterations::RansacIterations(
  const size_t & numberOfPoints,
  const float & fittingProbability,
  const size_t & maximalNumberOfIterations)
: logOfFittingOppositeProbability_(std::log(1.0 - fittingProbability)),
  oneOverNumberOfPoints_(1.0 / static_cast<double>(numberOfPoints)),
  numberOfIterations_(maximalNumberOfIterations)
{
}

//-----------------------------------------------------------------------------
void RansacIterations::update(
  const size_t & numberOfInliers,
  const size_t & numberOfPointsToDrawModel)
{
  double outliersPercentage = static_cast<double>(numberOfInliers) * oneOverNumberOfPoints_;
  double outliersProbability = 1.0 -
    std::pow(outliersPercentage, static_cast<double>(numberOfPointsToDrawModel));
  outliersProbability = std::max(EPSILON, outliersProbability);       // Avoid division by -Inf
  outliersProbability = std::min(1.0 - EPSILON, outliersProbability);  // Avoid division by 0.
  size_t numberOfIterations = logOfFittingOppositeProbability_ / std::log(outliersProbability);
  numberOfIterations_ = std::min(numberOfIterations_, static_cast<double>(numberOfIterations));
}

//-----------------------------------------------------------------------------
double RansacIterations::get() const
{
  return numberOfIterations_;
}

}  // namespace core
}  // namespace romea
