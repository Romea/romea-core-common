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


#ifndef ROMEA_CORE_COMMON__CONTAINERS__BOUNDINGBOX__AXISALIGNEDBOUNDINGBOX_HPP_
#define ROMEA_CORE_COMMON__CONTAINERS__BOUNDINGBOX__AXISALIGNEDBOUNDINGBOX_HPP_

#include  "romea_core_common/pointset/PointSet.hpp"
#include  "romea_core_common/pointset/PointTraits.hpp"
#include  "romea_core_common/math/Interval.hpp"

namespace romea
{
namespace core
{

template<typename Scalar, size_t DIM>
class AxisAlignedBoundingBox
{
public:
  using PointType = Eigen::Matrix<Scalar, DIM, 1>;
  using IntervalType = Interval<Scalar, DIM>;

public:
  AxisAlignedBoundingBox();

  explicit AxisAlignedBoundingBox(const IntervalType & extremities);

  AxisAlignedBoundingBox(
    const PointType & centerPosition,
    const PointType & halfWidthExtents);

  virtual ~AxisAlignedBoundingBox() = default;

public:
  const PointType & getCenterPosition()const;

  const PointType & getHalfWidthExtents()const;

  virtual IntervalType toInterval()const;

  virtual bool isInside(const PointType & point) const;

protected:
  PointType centerPosition_;
  PointType halfWidthExtents_;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF_VECTORIZABLE_FIXED_SIZE(Scalar, DIM)
};


typedef AxisAlignedBoundingBox<float, 2> AxisAlignedBoundingBox2f;
typedef AxisAlignedBoundingBox<float, 3> AxisAlignedBoundingBox3f;
typedef AxisAlignedBoundingBox<double, 2> AxisAlignedBoundingBox2d;
typedef AxisAlignedBoundingBox<double, 3> AxisAlignedBoundingBox3d;

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__CONTAINERS__BOUNDINGBOX__AXISALIGNEDBOUNDINGBOX_HPP_
