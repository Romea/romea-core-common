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

// romea
#include "romea_core_common/pointset/kdtree/KdMapNode.hpp"

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
template<class PointType>
KdMapNode<PointType>::KdMapNode(const PointType & center)
: center_(center)
{
}

//-----------------------------------------------------------------------------
template<class PointType>
KdMapNode<PointType>::~KdMapNode()
{
}

// TODO(Jean) factoriser en utilisant if constexpr sur POINT::DIM
//-----------------------------------------------------------------------------
template<>
size_t
KdMapNode<CartesianCoordinates3f>::getChildIndex(const CartesianCoordinates3f & point) const
{
  size_t index = 0;
  if (point.x() >= center_.x()) {index |= 4;}
  if (point.y() >= center_.y()) {index |= 2;}
  if (point.z() >= center_.z()) {index |= 1;}
  return index;
}

//-----------------------------------------------------------------------------
template<>
size_t
KdMapNode<HomogeneousCoordinates3f>::getChildIndex(const HomogeneousCoordinates3f & point) const
{
  size_t index = 0;
  if (point.x() >= center_.x()) {index |= 4;}
  if (point.y() >= center_.y()) {index |= 2;}
  if (point.z() >= center_.z()) {index |= 1;}
  return index;
}

//-----------------------------------------------------------------------------
template<>
size_t
KdMapNode<CartesianCoordinates3d>::getChildIndex(const CartesianCoordinates3d & point) const
{
  size_t index = 0;
  if (point.x() >= center_.x()) {index |= 4;}
  if (point.y() >= center_.y()) {index |= 2;}
  if (point.z() >= center_.z()) {index |= 1;}
  return index;
}

//-----------------------------------------------------------------------------
template<>
size_t
KdMapNode<HomogeneousCoordinates3d>::getChildIndex(const HomogeneousCoordinates3d & point) const
{
  size_t index = 0;
  if (point.x() >= center_.x()) {index |= 4;}
  if (point.y() >= center_.y()) {index |= 2;}
  if (point.z() >= center_.z()) {index |= 1;}
  return index;
}

//-----------------------------------------------------------------------------
template<>
size_t
KdMapNode<Eigen::Vector4f>::getChildIndex(const Eigen::Vector4f & point) const
{
  size_t index = 0;
  if (point.x() >= center_.x()) {index |= 4;}
  if (point.y() >= center_.y()) {index |= 2;}
  if (point.z() >= center_.z()) {index |= 1;}
  return index;
}

//-----------------------------------------------------------------------------
template<>
size_t
KdMapNode<Eigen::Vector4d>::getChildIndex(const Eigen::Vector4d & point) const
{
  size_t index = 0;
  if (point.x() >= center_.x()) {index |= 4;}
  if (point.y() >= center_.y()) {index |= 2;}
  if (point.z() >= center_.z()) {index |= 1;}
  return index;
}


//-----------------------------------------------------------------------------
template<>
size_t
KdMapNode<CartesianCoordinates2d>::getChildIndex(const CartesianCoordinates2d & point) const
{
  size_t index = 0;
  if (point.x() >= center_.x()) {index |= 2;}
  if (point.y() >= center_.y()) {index |= 1;}
  return index;
}

//-----------------------------------------------------------------------------
template<>
size_t
KdMapNode<CartesianCoordinates2f>::getChildIndex(const CartesianCoordinates2f & point) const
{
  size_t index = 0;
  if (point.x() >= center_.x()) {index |= 2;}
  if (point.y() >= center_.y()) {index |= 1;}
  return index;
}

//-----------------------------------------------------------------------------
template<>
size_t
KdMapNode<HomogeneousCoordinates2f>::getChildIndex(const HomogeneousCoordinates2f & point) const
{
  size_t index = 0;
  if (point.x() >= center_.x()) {index |= 2;}
  if (point.y() >= center_.y()) {index |= 1;}
  return index;
}

//-----------------------------------------------------------------------------
template<>
size_t
KdMapNode<HomogeneousCoordinates2d>::getChildIndex(const HomogeneousCoordinates2d & point) const
{
  size_t index = 0;
  if (point.x() >= center_.x()) {index |= 2;}
  if (point.y() >= center_.y()) {index |= 1;}
  return index;
}


//-----------------------------------------------------------------------------
template<class PointType>
const PointType & KdMapNode<PointType>::getCenter()const
{
  return center_;
}

template class KdMapNode<CartesianCoordinates2f>;
template class KdMapNode<CartesianCoordinates2d>;
template class KdMapNode<CartesianCoordinates3f>;
template class KdMapNode<CartesianCoordinates3d>;

template class KdMapNode<HomogeneousCoordinates2f>;
template class KdMapNode<HomogeneousCoordinates2d>;
template class KdMapNode<HomogeneousCoordinates3f>;
template class KdMapNode<HomogeneousCoordinates3d>;
template class KdMapNode<Eigen::Vector4f>;
template class KdMapNode<Eigen::Vector4d>;


}  // namespace core
}  // namespace romea
