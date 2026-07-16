// Copyright 2022 INRAE, French National Research Institute for Agriculture,
// Food and Environment
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

#ifndef ROMEA_CORE_COMMON__REGRESSION__RANSAC__RANSACRANDOMCORRESPONDENCES_HPP_
#define ROMEA_CORE_COMMON__REGRESSION__RANSAC__RANSACRANDOMCORRESPONDENCES_HPP_

// Eigen
#include <Eigen/Eigen>
#include <unsupported/Eigen/MatrixFunctions>

// stl
#include <random>
#include <vector>

// romea
#include "romea_core_common/pointset/PointSet.hpp"
#include "romea_core_common/pointset/PointTraits.hpp"
#include "romea_core_common/pointset/algorithms/Correspondence.hpp"

namespace romea {
namespace core {

template <class PointType>
class RansacRandomCorrespondences {
 public:
  RansacRandomCorrespondences();

  void computeScale(const PointType& pointSetMin, const PointType& pointSetMax);

  std::vector<Correspondence> drawPoints(
      const PointSet<PointType>& sourcePointSet,
      const std::vector<Correspondence>& correspondences,
      const size_t& numberOfRandomCorrespondences);

 private:
  void resetWeights_();

  void computeCumSumWeights_();

  void updateWeights_(const PointSet<PointType>& preconditionedSourcePointSet,
                      const std::vector<Correspondence>& correspondences,
                      const size_t& correspondenceIndex);

 private:
  size_t numberOfCorrespondences_;

  PointType scale_;
  std::vector<double> weights_;
  std::vector<double> cum_sum_weights_;

  std::default_random_engine randomGenerator_;
  std::uniform_real_distribution<double> uniformDistribution_;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF_VECTORIZABLE_FIXED_SIZE(
      typename PointType::Scalar, PointTraits<PointType>::SIZE)
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__REGRESSION__RANSAC__RANSACRANDOMCORRESPONDENCES_HPP_
