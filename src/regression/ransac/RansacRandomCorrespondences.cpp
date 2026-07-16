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

// romea
#include "romea_core_common/regression/ransac/RansacRandomCorrespondences.hpp"

// std
#include <algorithm>
#include <functional>
#include <vector>

namespace romea {
namespace core {

//-----------------------------------------------------------------------------
template <class PointType>
RansacRandomCorrespondences<PointType>::RansacRandomCorrespondences()
    : numberOfCorrespondences_(0),
      scale_(PointType::Zero()),
      weights_(),
      cum_sum_weights_(),
      randomGenerator_(),
      uniformDistribution_(0.0, 1.0) {}

//-----------------------------------------------------------------------------
template <class PointType>
void RansacRandomCorrespondences<PointType>::computeScale(
    const PointType& pointSetMin, const PointType& pointSetMax) {
  scale_ = 2 * (pointSetMax - pointSetMin) / std::sqrt(12);
}

//-----------------------------------------------------------------------------
template <class PointType>
std::vector<Correspondence> RansacRandomCorrespondences<PointType>::drawPoints(
    const PointSet<PointType>& sourcePointSet,
    const std::vector<Correspondence>& correspondences,
    const size_t& numberOfRandomCorrespondences) {
  numberOfCorrespondences_ = correspondences.size();
  assert(numberOfCorrespondences_ > numberOfRandomCorrespondences);

  weights_.resize(numberOfCorrespondences_);
  cum_sum_weights_.resize(numberOfCorrespondences_);
  for (size_t n = 0; n < correspondences.size(); n++) {
    weights_[n] = correspondences[n].weight;
  }
  computeCumSumWeights_();

  // Draw randomly N correspondences
  std::vector<Correspondence> randomCorrespondences(
      numberOfRandomCorrespondences);

  for (size_t n = 1; n <= numberOfRandomCorrespondences; ++n) {
    double* I =
        std::lower_bound(cum_sum_weights_.data(),
                         cum_sum_weights_.data() + cum_sum_weights_.size(),
                         uniformDistribution_(randomGenerator_));

    size_t index = size_t(std::distance(cum_sum_weights_.data(), I));
    randomCorrespondences[n - 1] = correspondences[index];

    updateWeights_(sourcePointSet, correspondences, index);
  }

  return randomCorrespondences;
}

//-----------------------------------------------------------------------------
template <class PointType>
void RansacRandomCorrespondences<PointType>::resetWeights_() {
  weights_.assign(numberOfCorrespondences_, 1.);
  computeCumSumWeights_();
}

//-----------------------------------------------------------------------------
template <class PointType>
void RansacRandomCorrespondences<PointType>::updateWeights_(
    const PointSet<PointType>& preconditionedSourcePointSet,
    const std::vector<Correspondence>& correspondences,
    const size_t& correspondenceIndex) {
  const size_t& drawSourceIndex =
      correspondences[correspondenceIndex].sourcePointIndex;
  const size_t& drawTargetIndex =
      correspondences[correspondenceIndex].targetPointIndex;
  const PointType& drawSourcePoint =
      preconditionedSourcePointSet[drawSourceIndex];

  for (size_t n = 0; n < numberOfCorrespondences_; ++n) {
    const size_t& sourceIndex = correspondences[n].sourcePointIndex;
    const size_t& targetIndex = correspondences[n].targetPointIndex;
    if (targetIndex == drawTargetIndex) {
      weights_[n] = 0;
    } else {
      const PointType point = preconditionedSourcePointSet[sourceIndex];
      weights_[n] *=
          1 - std::exp(-((point - drawSourcePoint).array() * scale_.array())
                            .square()
                            .sum());
    }
  }

  computeCumSumWeights_();
}

//-----------------------------------------------------------------------------
template <class PointType>
void RansacRandomCorrespondences<PointType>::computeCumSumWeights_() {
  std::partial_sum(cbegin(weights_), std::cend(weights_),
                   std::begin(cum_sum_weights_));

  std::transform(std::begin(cum_sum_weights_), std::end(cum_sum_weights_),
                 std::begin(cum_sum_weights_),
                 std::bind(std::divides<double>(), std::placeholders::_1,
                           cum_sum_weights_.back()));
}

template class RansacRandomCorrespondences<Eigen::Vector2f>;
template class RansacRandomCorrespondences<Eigen::Vector2d>;
template class RansacRandomCorrespondences<Eigen::Vector3f>;
template class RansacRandomCorrespondences<Eigen::Vector3d>;

template class RansacRandomCorrespondences<HomogeneousCoordinates2f>;
template class RansacRandomCorrespondences<HomogeneousCoordinates2d>;
template class RansacRandomCorrespondences<HomogeneousCoordinates3f>;
template class RansacRandomCorrespondences<HomogeneousCoordinates3d>;

}  // namespace core
}  // namespace romea
