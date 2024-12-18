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

#ifndef ROMEA_CORE_COMMON__POINTSET__KDTREE__KDMAPBRANCH_HPP_
#define ROMEA_CORE_COMMON__POINTSET__KDTREE__KDMAPBRANCH_HPP_

// std
#include <vector>

// romea
#include "KdMapLeaf.hpp"

namespace romea
{
namespace core
{

template<class PointType>
class KdMapBranch : public KdMapNode<PointType>
{
public:
  typedef typename PointType::Scalar Scalar;
  static constexpr size_t POINT_SIZE = PointTraits<PointType>::SIZE;
  static constexpr size_t CARTESIAN_DIM = PointTraits<PointType>::DIM;

public:
  explicit KdMapBranch(const PointType & center);

  ~KdMapBranch();

public:
  virtual KdMapNodeType getType() const;

  KdMapNode<PointType> * & getChild(const size_t & childIndex);

  void createChild(const size_t & childIndex, const Scalar & halfExtent);

private:
  std::vector<KdMapNode<PointType> *> children_;
};

}  // namespace core
}  // namespace romea


namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
template<class PointType>
KdMapBranch<PointType>::KdMapBranch(const PointType & center)
: KdMapNode<PointType>(center),
  children_(std::pow(2, CARTESIAN_DIM), nullptr)
{
}

//-----------------------------------------------------------------------------
template<class PointType>
KdMapNodeType
KdMapBranch<PointType>::getType() const
{
  return BRANCH_NODE;
}

//-----------------------------------------------------------------------------
template<class PointType>
KdMapNode<PointType> * &
KdMapBranch<PointType>::getChild(const size_t & childIndex)
{
  return children_[childIndex];
}

//-----------------------------------------------------------------------------
template<class PointType>
KdMapBranch<PointType>::~KdMapBranch()
{
  for (KdMapNode<PointType> * child : children_) {
    if (child) {delete (child);}}
}

// TODO(Jean) factoriser en utilisant les if constexpr sur PointType DIM
//-----------------------------------------------------------------------------
//    Children follow a predictable pattern to make accesses simple.
//    Here, - means less than 'center' in that dimension, + means greater than.
//    child:  0 1 2 3
//    x:      - - + +
//    y:      - + - +

template<>
inline
void KdMapBranch<CartesianCoordinates2d>::createChild(
  const size_t & childIndex,
  const double & halfExtent)
{
  CartesianCoordinates2d center = center_;
  center.x() += halfExtent * (childIndex & 2 ? 1. : -1.);
  center.y() += halfExtent * (childIndex & 1 ? 1. : -1.);
  children_[childIndex] = new KdMapLeaf<CartesianCoordinates2d>(center);
}

template<>
inline
void KdMapBranch<CartesianCoordinates2f>::createChild(
  const size_t & childIndex,
  const float & halfExtent)
{
  CartesianCoordinates2f center = center_;
  center.x() += halfExtent * (childIndex & 2 ? 1.f : -1.f);
  center.y() += halfExtent * (childIndex & 1 ? 1.f : -1.f);
  children_[childIndex] = new KdMapLeaf<CartesianCoordinates2f>(center);
}

template<>
inline
void KdMapBranch<HomogeneousCoordinates2d>::createChild(
  const size_t & childIndex,
  const double & halfExtent)
{
  HomogeneousCoordinates2d center = center_;
  center.x() += halfExtent * (childIndex & 2 ? 1. : -1.);
  center.y() += halfExtent * (childIndex & 1 ? 1. : -1.);
  center.z() = 1;
  children_[childIndex] = new KdMapLeaf<HomogeneousCoordinates2d>(center);
}

template<>
inline
void KdMapBranch<HomogeneousCoordinates2f>::createChild(
  const size_t & childIndex,
  const float & halfExtent)
{
  HomogeneousCoordinates2f center = center_;
  center.x() += halfExtent * (childIndex & 2 ? 1.f : -1.f);
  center.y() += halfExtent * (childIndex & 1 ? 1.f : -1.f);
  center.z() = 1;
  children_[childIndex] = new KdMapLeaf<HomogeneousCoordinates2f>(center);
}

//-----------------------------------------------------------------------------
//    Children follow a predictable pattern to make accesses simple.
//    Here, - means less than 'center' in that dimension, + means greater than.
//    child:  0 1 2 3 4 5 6 7
//    x:      - - - - + + + +
//    y:      - - + + - - + +
//    z:      - + - + - + - +

template<>
inline
void KdMapBranch<CartesianCoordinates3d>::createChild(
  const size_t & childIndex,
  const double & halfExtent)
{
  CartesianCoordinates3d center = center_;
  center.x() += halfExtent * (childIndex & 4 ? 1 : -1);
  center.y() += halfExtent * (childIndex & 2 ? 1 : -1);
  center.z() += halfExtent * (childIndex & 1 ? 1 : -1);
  children_[childIndex] = new KdMapLeaf<CartesianCoordinates3d>(center);
}

template<>
inline
void KdMapBranch<CartesianCoordinates3f>::createChild(
  const size_t & childIndex,
  const float & halfExtent)
{
  CartesianCoordinates3f center = center_;
  center.x() += halfExtent * (childIndex & 4 ? 1.f : -1.f);
  center.y() += halfExtent * (childIndex & 2 ? 1.f : -1.f);
  center.z() += halfExtent * (childIndex & 1 ? 1.f : -1.f);
  children_[childIndex] = new KdMapLeaf<CartesianCoordinates3f>(center);
}

template<>
inline
void KdMapBranch<HomogeneousCoordinates3d>::createChild(
  const size_t & childIndex,
  const double & halfExtent)
{
  HomogeneousCoordinates3d center = center_;
  center.x() += halfExtent * (childIndex & 4 ? 1 : -1);
  center.y() += halfExtent * (childIndex & 2 ? 1 : -1);
  center.z() += halfExtent * (childIndex & 1 ? 1 : -1);
  center.w() = 1;
  children_[childIndex] = new KdMapLeaf<HomogeneousCoordinates3d>(center);
}

template<>
inline
void KdMapBranch<HomogeneousCoordinates3f>::createChild(
  const size_t & childIndex,
  const float & halfExtent)
{
  HomogeneousCoordinates3f center = center_;
  center.x() += halfExtent * (childIndex & 4 ? 1.f : -1.f);
  center.y() += halfExtent * (childIndex & 2 ? 1.f : -1.f);
  center.z() += halfExtent * (childIndex & 1 ? 1.f : -1.f);
  center.w() = 1;
  children_[childIndex] = new KdMapLeaf<HomogeneousCoordinates3f>(center);
}

template<>
inline
void KdMapBranch<Eigen::Vector4f>::createChild(
  const size_t & childIndex,
  const float & halfExtent)
{
  Eigen::Vector4f center = center_;
  center.x() += halfExtent * (childIndex & 4 ? 1.f : -1.f);
  center.y() += halfExtent * (childIndex & 2 ? 1.f : -1.f);
  center.z() += halfExtent * (childIndex & 1 ? 1.f : -1.f);
  center.w() = 1;
  children_[childIndex] = new KdMapLeaf<Eigen::Vector4f>(center);
}

template<>
inline
void KdMapBranch<Eigen::Vector4d>::createChild(
  const size_t & childIndex,
  const double & halfExtent)
{
  Eigen::Vector4d center = center_;
  center.x() += halfExtent * (childIndex & 4 ? 1.f : -1.f);
  center.y() += halfExtent * (childIndex & 2 ? 1.f : -1.f);
  center.z() += halfExtent * (childIndex & 1 ? 1.f : -1.f);
  center.w() = 1;
  children_[childIndex] = new KdMapLeaf<Eigen::Vector4d>(center);
}


}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__POINTSET__KDTREE__KDMAPBRANCH_HPP_
