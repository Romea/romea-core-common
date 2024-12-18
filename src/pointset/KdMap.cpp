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
#include <vector>
#include <iostream>

// romea
#include "romea_core_common/pointset/KdMap.hpp"

namespace
{
const double MAXIMAL_NEIGHBOR_VECTOR_SIZE = 16;
}

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
template<class PointType>
KdMap<PointType>::KdMap(
  const Scalar & resolution,
  const Scalar & maximalHalfExtent,
  const size_t & maximalBucketSize,
  const size_t & expectedNumberOfPoints)
: squaredResolution_(resolution * resolution),
  minimalHalfExtent_(),
  maximalHalfExtent_(maximalHalfExtent),
  maximalBucketSize_(maximalBucketSize),
  treeDepth_(0),
  treeCellRadius_(),
  treeCellHalExtents_(),
  researchPatterns_(),
  rootBranch_(nullptr),
  numberOfPoints_(expectedNumberOfPoints),
  points_()
{
  // compute half extent of minimal cell
  std::cout << maximalBucketSize << " " << Scalar(CARTESIAN_DIM) << " " << resolution << std::endl;
  minimalHalfExtent_ =
    std::ceil(std::pow(maximalBucketSize, 1 / Scalar(CARTESIAN_DIM))) * resolution;

  std::cout << "minimalHalfExtent_ " << minimalHalfExtent_ << std::endl;

  // compute tree depth
  while (maximalHalfExtent_ > minimalHalfExtent_) {
    ++treeDepth_;
    maximalHalfExtent_ /= 2;
  }
  std::cout << "treeDepth_" << treeDepth_ << std::endl;

  // compute cell half extent according tree depth
  treeCellHalExtents_.resize(treeDepth_ + 1);
  maximalHalfExtent_ = minimalHalfExtent_;
  for (size_t n = 0; n < treeDepth_; ++n) {
    treeCellHalExtents_[n] = maximalHalfExtent_;
    maximalHalfExtent_ *= 2;
  }
  treeCellHalExtents_[treeDepth_] = maximalHalfExtent_;

  // compute cell radius according tree depth
  treeCellRadius_.resize(treeDepth_ + 1);
  for (size_t n = 0; n < treeDepth_ + 1; ++n) {
    treeCellRadius_[n] = std::sqrt(CARTESIAN_DIM) * treeCellHalExtents_[n];
  }

  // allocate roor node
  rootBranch_ = new KdMapBranch<PointType>(PointType::Zero());

  // load research patterns
  loadResearchPatterns_();
}

//-----------------------------------------------------------------------------
template<class PointType>
KdMap<PointType>::~KdMap()
{
  delete rootBranch_;
}

//-----------------------------------------------------------------------------
template<class PointType>
bool KdMap<PointType>::insertPoint(const PointType & point)
{
  assert(point.norm() < treeCellRadius_[treeDepth_]);

  if (points_.size() == numberOfPoints_) {
    numberOfPoints_ *= 2;
    points_.reserve(numberOfPoints_);
  }

  // search leaf where the point is located
  KdMapLeaf<PointType> * leaf = nullptr;
  findLeaf_(point, treeDepth_, rootBranch_, leaf);

  assert(leaf != nullptr);

  // try to add point
  const size_t & leafIndexBucketSize = leaf->getIndexBucketSize();
  const std::vector<size_t> & leafIndexBucket = leaf->getIndexBucket();

  if (leafIndexBucketSize != maximalBucketSize_) {
    size_t n = 0;
    for (; n < leafIndexBucketSize; ++n) {
      if ((point - points_[leafIndexBucket[n]]).squaredNorm() < squaredResolution_) {
        break;
      }
    }

    if (n == leafIndexBucketSize) {
      leaf->addIndex(points_.size());
      points_.push_back(point);
      return true;
    }
  }

  return false;
}

//-----------------------------------------------------------------------------
template<class PointType>
void KdMap<PointType>::findLeaf_(
  const PointType & point,
  size_t treeDepth,
  BranchType * branch,
  LeafType * & leaf)
{
  size_t childIndex = branch->getChildIndex(point);
  NodeType * & childNode = branch->getChild(childIndex);

  if (childNode) {
    if (childNode->getType() == LEAF_NODE) {
      leaf = static_cast<LeafType *>(childNode);

      // expand leaf
      if (treeDepth && leaf->getIndexBucketSize() == maximalBucketSize_) {
        // create new branch
        BranchType * newBranch = new BranchType(leaf->getCenter());

        // fill new leaves
        const std::vector<size_t> & leafIndexBucket = leaf->getIndexBucket();
        for (const size_t & index : leafIndexBucket) {
          findLeaf_(points_[index], treeDepth - 1, newBranch, leaf);
          leaf->addIndex(index);
        }

        // iterative research of the leaf
        findLeaf_(point, treeDepth - 1, newBranch, leaf);

        // substitute the leaf by the new branch
        delete childNode;
        childNode = newBranch;
      }

    } else {
      // iterative research of the leaf
      findLeaf_(point, treeDepth - 1, static_cast<BranchType *>(childNode), leaf);
    }
  } else {
    // create new leaf where point is located
    branch->createChild(childIndex, treeCellHalExtents_[treeDepth - 1]);
    leaf = static_cast<LeafType *>(branch->getChild(childIndex));
  }
}

//-----------------------------------------------------------------------------
template<class PointType>
const PointType & KdMap<PointType>::operator[](const size_t & pointIndex)const
{
  return points_[pointIndex];
}


//-----------------------------------------------------------------------------
template<class PointType>
const PointSet<PointType> & KdMap<PointType>::getPointSet()const
{
  return points_;
}

//-----------------------------------------------------------------------------
template<class PointType>
bool KdMap<PointType>::isEmpty()const
{
  return points_.empty();
}


//-----------------------------------------------------------------------------
template<class PointType>
template<class NeighborType>
void KdMap<PointType>::findNearestNeighbors_(
  const PointType & point,
  size_t treeDepth,
  BranchType * branch,
  NeighborType & neighbors) const
{
  const Scalar cellRadius = treeCellRadius_[treeDepth];

  for (const size_t & childIndex : researchPatterns_[branch->getChildIndex(point)]) {
    NodeType * childNode = branch->getChild(childIndex);

    if (childNode) {
      // Check if it's worth to research canditats in the child node
      if ((point - childNode->getCenter()).norm() >
        std::sqrt(neighbors.getWorstSquareDistance()) + cellRadius)
      {
        break;
      }

      if (childNode->getType() == LEAF_NODE) {
        // try to add points containing in the leaf in canditats
        LeafType * leaf = static_cast<LeafType *>(childNode);
        for (const size_t & index : leaf->getIndexBucket()) {
          Scalar squaredDistance = (point - points_[index]).squaredNorm();
          if (squaredDistance < neighbors.getWorstSquareDistance()) {
            neighbors.insert(index, squaredDistance);
          }
        }

      } else {
        // recursive nearest neighbor research
        findNearestNeighbors_(
          point, treeDepth - 1, static_cast<BranchType *>(childNode),
          neighbors);
      }
    }
  }
}

//-----------------------------------------------------------------------------
template<class PointType>
void KdMap<PointType>::findNearestNeighbors(
  const PointType & point,
  const size_t & numberOfNeighbors,
  std::vector<size_t> & neighborIndexes,
  std::vector<Scalar> & neighborSquaredDistances)const
{
  assert(numberOfNeighbors > 1);
  assert(numberOfNeighbors < points_.size());

  neighborIndexes.clear();
  neighborSquaredDistances.clear();

  if (maximalBucketSize_ > MAXIMAL_NEIGHBOR_VECTOR_SIZE) {
    NeighborHeapType neighborHeap(numberOfNeighbors);
    findNearestNeighbors_(point, treeDepth_, rootBranch_, neighborHeap);
    neighborHeap.sort();

    for (const auto & neighbor : neighborHeap.getNeighbors()) {
      neighborIndexes.push_back(neighbor.pointIndex);
      neighborSquaredDistances.push_back(neighbor.pointSquaredDistance);
    }
  } else {
    NeighborVectorType neighborVector(numberOfNeighbors);
    findNearestNeighbors_(point, treeDepth_, rootBranch_, neighborVector);
    neighborVector.sort();

    for (const auto & neighbor : neighborVector.getNeighbors()) {
      neighborIndexes.push_back(neighbor.pointIndex);
      neighborSquaredDistances.push_back(neighbor.pointSquaredDistance);
    }
  }
}


//-----------------------------------------------------------------------------
template<class PointType>
void KdMap<PointType>::findNearestNeighbor(
  const PointType & point,
  size_t & neighborIndex,
  Scalar & neighborSquareDistance)const
{
  assert(!points_.empty());

  neighborIndex = 0;
  neighborSquareDistance = std::numeric_limits<Scalar>::infinity();
  findNearestNeighbor_(point, treeDepth_, rootBranch_, neighborIndex, neighborSquareDistance);
}


//-----------------------------------------------------------------------------
template<class PointType>
void KdMap<PointType>::findNearestNeighbor_(
  const PointType & point,
  size_t treeDepth,
  BranchType * branch,
  size_t & neighborIndex,
  Scalar & neighborSquaredDistance)const
{
  const Scalar cellRadius = treeCellRadius_[treeDepth];

  for (const size_t & childIndex : researchPatterns_[branch->getChildIndex(point)]) {
    NodeType * childNode = branch->getChild(childIndex);

    if (childNode) {
      // Check if it's worth to research canditats in the child node
      if ((point - childNode->getCenter()).norm() >
        std::sqrt(neighborSquaredDistance) + cellRadius)
      {
        break;
      }

      if (childNode->getType() == LEAF_NODE) {
        // try to add points containing in the leaf in canditats
        LeafType * leaf = static_cast<LeafType *>(childNode);
        for (const size_t & index : leaf->getIndexBucket()) {
          Scalar squaredDistance = (point - points_[index]).squaredNorm();
          if (squaredDistance < neighborSquaredDistance) {
            neighborIndex = index;
            neighborSquaredDistance = squaredDistance;
          }
        }

      } else {
        // recursive nearest neighbor research
        findNearestNeighbor_(
          point, treeDepth - 1,
          static_cast<BranchType *>(childNode),
          neighborIndex, neighborSquaredDistance);
      }
    }
  }
}

//-----------------------------------------------------------------------------
template<class PointType>
void KdMap<PointType>::loadResearchPatterns_()
{
  if (CARTESIAN_DIM == 2) {
    researchPatterns_ = {
      {0, 1, 2, 3},
      {1, 0, 3, 2},
      {2, 3, 0, 1},
      {3, 2, 1, 0}};
  } else {
    researchPatterns_ = {
      {0, 2, 4, 1, 6, 3, 5, 7},
      {1, 3, 5, 0, 7, 2, 4, 6},
      {2, 0, 6, 3, 4, 1, 7, 5},
      {3, 1, 7, 2, 5, 0, 6, 4},
      {4, 6, 0, 5, 2, 7, 1, 3},
      {5, 7, 1, 4, 3, 6, 0, 2},
      {6, 4, 2, 7, 0, 5, 3, 1},
      {7, 5, 3, 6, 1, 4, 2, 0}};
  }
}

template class KdMap<CartesianCoordinates2f>;
template class KdMap<CartesianCoordinates2d>;
template class KdMap<CartesianCoordinates3f>;
template class KdMap<CartesianCoordinates3d>;

template class KdMap<HomogeneousCoordinates2f>;
template class KdMap<HomogeneousCoordinates2d>;
template class KdMap<HomogeneousCoordinates3f>;
template class KdMap<HomogeneousCoordinates3d>;
template class KdMap<Eigen::Vector4f>;
template class KdMap<Eigen::Vector4d>;


}  // namespace core
}  // namespace romea
