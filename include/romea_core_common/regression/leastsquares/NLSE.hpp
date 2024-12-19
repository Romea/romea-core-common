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


#ifndef ROMEA_CORE_COMMON__REGRESSION__LEASTSQUARES__NLSE_HPP_
#define ROMEA_CORE_COMMON__REGRESSION__LEASTSQUARES__NLSE_HPP_

#include "romea_core_common/regression/leastsquares/LeastSquares.hpp"

namespace romea
{
namespace core
{

template<typename RealType>
class NLSE
{
public:
  using Vector = Eigen::Matrix<RealType, Eigen::Dynamic, 1>;
  using Matrix = Eigen::Matrix<RealType, Eigen::Dynamic, Eigen::Dynamic>;

public:
  NLSE();

  explicit NLSE(const double & estimateEpsilon);

  NLSE(const double & estimateEpsilon, const RealType & alpha);

  virtual ~NLSE() = default;

public:
  virtual bool estimate(
    const size_t & maximalNumberOfIterations,
    const double & dataStd);

  const size_t & getNumberOfIterations();

  const double & getRootMeanSquareError();

  const Vector & getEstimate();

  const Matrix & getEstimateCovariance();

protected:
  virtual void computeGuess_() = 0;

  virtual void computeJacobianAndY_() = 0;

protected:
  double alpha_;
  Vector estimate_;
  Vector estimateDelta_;
  Matrix estimateCovariance_;
  double estimateEpsilon_;
  size_t numberOfIterations_;
  LeastSquares<RealType> leastSquares_;
  double rmse_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__REGRESSION__LEASTSQUARES__NLSE_HPP_
