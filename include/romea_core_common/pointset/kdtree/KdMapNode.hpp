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

#ifndef ROMEA_CORE_COMMON__POINTSET__KDTREE__KDMAPNODE_HPP_
#define ROMEA_CORE_COMMON__POINTSET__KDTREE__KDMAPNODE_HPP_

// std
#include <vector>
#include <memory>

// romea
#include "romea_core_common/pointset/PointSet.hpp"
#include "romea_core_common/pointset/PointTraits.hpp"


namespace romea
{
namespace core
{

enum KdMapNodeType
{
  BRANCH_NODE = 0,
  LEAF_NODE
};

// TODO(Jean) use crtp optimization
template<class PointType>
class KdMapNode
{
public:
  explicit KdMapNode(const PointType & center);

  virtual ~KdMapNode();

public:
  virtual KdMapNodeType getType() const = 0;

  size_t getChildIndex(const PointType & point) const;

  const PointType & getCenter()const;

protected:
  PointType center_;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF_VECTORIZABLE_FIXED_SIZE(
    typename PointType::Scalar, PointTraits<PointType>::SIZE);
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__POINTSET__KDTREE__KDMAPNODE_HPP_
