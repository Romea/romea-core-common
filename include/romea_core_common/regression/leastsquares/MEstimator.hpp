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


#ifndef ROMEA_CORE_COMMON__REGRESSION__LEASTSQUARES__MESTIMATOR_HPP_
#define ROMEA_CORE_COMMON__REGRESSION__LEASTSQUARES__MESTIMATOR_HPP_

// Eigen
#include <Eigen/Eigen>

// std
#include <limits>

namespace romea
{
namespace core
{

const float MESTIMATOR_DISCARDED_VALUE_FLOAT = std::numeric_limits<float>::max();
const double MESTIMATOR_DISCARDED_VALUE_DOUBLE = std::numeric_limits<double>::max();

template<class RealType>
class MEstimator
{
public:
  using Vector = Eigen::Matrix<RealType, Eigen::Dynamic, 1>;

  explicit MEstimator(RealType dataNoiseStd);

  RealType computeWeights(const Vector & residuals);

  RealType computeWeights(
    const Vector & residualsWithDiscardedValue,
    const size_t & numberOfDiscardedData);

  const Vector & getWeights()const { return weights_; }

private:
  void allocate_(const int & dataSize);

private:
  int dataSize_;
  RealType dataNoiseStd_;
  Vector sortedVector_;
  Vector normalizedResiduals_;
  Vector weights_;
  Vector ones_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__REGRESSION__LEASTSQUARES__MESTIMATOR_HPP_
