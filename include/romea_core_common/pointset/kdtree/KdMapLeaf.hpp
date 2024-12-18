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

#ifndef ROMEA_CORE_COMMON__POINTSET__KDTREE__KDMAPLEAF_HPP_
#define ROMEA_CORE_COMMON__POINTSET__KDTREE__KDMAPLEAF_HPP_

// std
#include <vector>

// romea
#include "KdMapNode.hpp"

namespace romea
{
namespace core
{

template<class PointType>
class KdMapLeaf : public KdMapNode<PointType>
{
public:
  explicit KdMapLeaf(const PointType & center);

  virtual KdMapNodeType getType() const;

public:
  const std::vector<size_t> & getIndexBucket() const;

  void addIndex(const size_t & pointIndex);

  size_t getIndexBucketSize() const;

private:
  std::vector<size_t> indexBucket_;
};


//-----------------------------------------------------------------------------
template<class PointType>
KdMapLeaf<PointType>::KdMapLeaf(const PointType & center)
: KdMapNode<PointType>(center),
  indexBucket_()
{
}

//-----------------------------------------------------------------------------
template<class PointType>
KdMapNodeType
KdMapLeaf<PointType>::getType() const
{
  return LEAF_NODE;
}

//-----------------------------------------------------------------------------
template<class PointType>
void
KdMapLeaf<PointType>::addIndex(const size_t & pointIndex)
{
  indexBucket_.push_back(pointIndex);
}

//-----------------------------------------------------------------------------
template<class PointType>
const std::vector<size_t> &
KdMapLeaf<PointType>::getIndexBucket() const
{
  return indexBucket_;
}

//-----------------------------------------------------------------------------
template<class PointType>
size_t
KdMapLeaf<PointType>::getIndexBucketSize() const
{
  return indexBucket_.size();
}

}  // namespace core
}  // namespace romea


#endif  // ROMEA_CORE_COMMON__POINTSET__KDTREE__KDMAPLEAF_HPP_
