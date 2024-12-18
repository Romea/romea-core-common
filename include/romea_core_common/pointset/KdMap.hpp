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

#ifndef ROMEA_CORE_COMMON__POINTSET__KDMAP_HPP_
#define ROMEA_CORE_COMMON__POINTSET__KDMAP_HPP_

// std
#include <vector>

// romea
#include "romea_core_common/pointset/PointSet.hpp"
#include "romea_core_common/pointset/PointTraits.hpp"
#include "romea_core_common/pointset/kdtree/KdMapBranch.hpp"
#include "romea_core_common/pointset/kdtree/NeighborHeap.hpp"
#include "romea_core_common/pointset/kdtree/NeighborVector.hpp"

namespace romea
{
namespace core
{

template<class PointType>
class KdMap
{
  typedef typename PointType::Scalar Scalar;
  static constexpr size_t POINT_SIZE = PointTraits<PointType>::SIZE;
  static constexpr size_t CARTESIAN_DIM = PointTraits<PointType>::DIM;

  typedef KdMapNode<PointType> NodeType;
  typedef KdMapBranch<PointType> BranchType;
  typedef KdMapLeaf<PointType> LeafType;
  typedef NeighborHeap<Scalar> NeighborHeapType;
  typedef NeighborVector<Scalar> NeighborVectorType;

public:
  KdMap(
    const Scalar & resolution,
    const Scalar & maximalHalfExtent,
    const size_t & maximalBucketSize,
    const size_t & expectedNumberOfPoints);

  KdMap(const KdMap &) = delete;

  KdMap & operator=(const KdMap &) = delete;

  ~KdMap();

public:
  bool insertPoint(const PointType & point);

  const PointSet<PointType> & getPointSet()const;

  virtual void findNearestNeighbor(
    const PointType & point,
    size_t & neighborIndex,
    Scalar & neighborSquareDistance)const;

  virtual void findNearestNeighbors(
    const PointType & point,
    const size_t & numberOfNeighbors,
    std::vector<size_t> & neighborIndexes,
    std::vector<Scalar> & neighborSquareDistances)const;

  const PointType & operator[](const size_t & pointIndex)const;

  bool isEmpty()const;

private:
  void loadResearchPatterns_();

  void findLeaf_(
    const PointType & point,
    size_t treeDepth,
    BranchType * branch,
    LeafType * & leaf);


  void findNearestNeighbor_(
    const PointType & point,
    size_t treeDepth,
    BranchType * branch,
    size_t & neighborIndex,
    Scalar & neighborSquaredDistance)const;

  template<class NeighborType>
  void findNearestNeighbors_(
    const PointType & point,
    size_t treeDepth,
    BranchType * branch,
    NeighborType & neighbors)const;

private:
  Scalar squaredResolution_;
  Scalar minimalHalfExtent_;
  Scalar maximalHalfExtent_;
  size_t maximalBucketSize_;

  size_t treeDepth_;
  std::vector<typename PointType::Scalar> treeCellRadius_;
  std::vector<typename PointType::Scalar> treeCellHalExtents_;
  std::vector<std::vector<size_t>> researchPatterns_;

  BranchType * rootBranch_;

  size_t numberOfPoints_;
  PointSet<PointType> points_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__POINTSET__KDMAP_HPP_
