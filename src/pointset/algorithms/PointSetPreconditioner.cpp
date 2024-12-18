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


// std
#include <limits>

// romea
#include "romea_core_common/pointset/algorithms/PointSetPreconditioner.hpp"

namespace romea
{

namespace core
{

//-----------------------------------------------------------------------------
template<class PointType>
PointSetPreconditioner<PointType>::PointSetPreconditioner()
: scale_(0),
  translation_(TranslationVector::Zero()),
  pointSetMean_(PointType::Zero()),
  pointSetMin_(PointType::Zero()),
  pointSetMax_(PointType::Zero())
{
}

//-----------------------------------------------------------------------------
template<class PointType>
PointSetPreconditioner<PointType>::PointSetPreconditioner(const PointSet<PointType> & points)
: PointSetPreconditioner<PointType>::PointSetPreconditioner()
{
  compute(points);
}

//-----------------------------------------------------------------------------
template<class PointType>
void
PointSetPreconditioner<PointType>::compute(const PointSet<PointType> & points)
{
  // Compute mean and scales of point set
  pointSetMean_.setConstant(0);
  pointSetMin_.setConstant(std::numeric_limits<Scalar>::max());
  pointSetMax_.setConstant(std::numeric_limits<Scalar>::min());
  for (size_t n = 0, N = points.size(); n < N; ++n) {
    const PointType & point = points[n];
    pointSetMin_.array() = pointSetMin_.array().min(point.array());
    pointSetMax_.array() = pointSetMax_.array().max(point.array());
    pointSetMean_ += point;
  }
  pointSetMean_ /= static_cast<int>(points.size());


  // Scale min, max and mean
  scale_ = 1 / (pointSetMax_ - pointSetMin_).maxCoeff();
  translation_ = -pointSetMean_.head(CARTESIAN_DIM) * scale_;
}

//-----------------------------------------------------------------------------
template<class PointType>
const PointType & PointSetPreconditioner<PointType>::getPointSetMean()const
{
  return pointSetMean_;
}

//-----------------------------------------------------------------------------
template<class PointType>
const PointType & PointSetPreconditioner<PointType>::getPointSetMin()const
{
  return pointSetMin_;
}

//-----------------------------------------------------------------------------
template<class PointType>
const PointType & PointSetPreconditioner<PointType>::getPointSetMax()const
{
  return pointSetMax_;
}


//-----------------------------------------------------------------------------
template<class PointType>
const typename PointType::Scalar &
PointSetPreconditioner<PointType>::getScale()const
{
  return scale_;
}

//-----------------------------------------------------------------------------
template<class PointType>
const typename PointSetPreconditioner<PointType>::TranslationVector &
PointSetPreconditioner<PointType>::getTranslation() const
{
  return translation_;
}

template class PointSetPreconditioner<Eigen::Vector2f>;
template class PointSetPreconditioner<Eigen::Vector2d>;
template class PointSetPreconditioner<Eigen::Vector3f>;
template class PointSetPreconditioner<Eigen::Vector3d>;

template class PointSetPreconditioner<HomogeneousCoordinates2f>;
template class PointSetPreconditioner<HomogeneousCoordinates2d>;
template class PointSetPreconditioner<HomogeneousCoordinates3f>;
template class PointSetPreconditioner<HomogeneousCoordinates3d>;

}  // namespace core
}  // namespace romea
