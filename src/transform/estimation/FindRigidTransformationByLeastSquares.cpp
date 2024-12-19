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

// romea
#include "romea_core_common/transform/estimation/FindRigidTransformationByLeastSquares.hpp"

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
template<class PointType>
FindRigidTransformationByLeastSquares<PointType>::FindRigidTransformationByLeastSquares()
: leastSquares_()
{
  if (CARTESIAN_DIM == 2) {
    leastSquares_.setEstimateSize(3);
  } else {
    leastSquares_.setEstimateSize(6);
  }
}

//-----------------------------------------------------------------------------
template<class PointType>
void
FindRigidTransformationByLeastSquares<PointType>::setPreconditioner(
  const PreconditionedPointSetType &
#ifndef NDEBUG
  sourcePoints
#endif
  , const PreconditionedPointSetType & targetPoints)
{
  assert(
    (sourcePoints.getPreconditioningMatrix() -
    targetPoints.getPreconditioningMatrix()).array().sum() <
    std::numeric_limits<Scalar>::epsilon());

  Scalar scale = targetPoints.getPreconditioningMatrix()(0, 0);
  Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> Ac;

  if (CARTESIAN_DIM == 2) {
    Ac = Eigen::Matrix<Scalar, 3, 3>::Identity();
  } else {
    Ac = Eigen::Matrix<Scalar, 6, 6>::Identity();
  }

  Ac.block(0, 0, CARTESIAN_DIM, CARTESIAN_DIM) /= scale;
  leastSquares_.setPreconditionner(Ac);
}

// TODO(jean) a factoriser
//-----------------------------------------------------------------------------
template<class PointType>
typename FindRigidTransformationByLeastSquares<PointType>::TransformationMatrixType
FindRigidTransformationByLeastSquares<PointType>::estimate_(
  const PointSet<PointType> & sourcePoints,
  const PointSet<PointType> & targetPoints,
  const NormalSet<PointType> & targetPointsNormals,
  const std::vector<Correspondence> & correspondences)
{
  // Allocate least squares matrices
  size_t numberOfPoints = correspondences.size();
  leastSquares_.setDataSize(numberOfPoints);

  // Compute Jacobian and residuals
  auto & J = leastSquares_.getJ();
  auto & Y = leastSquares_.getY();

  Eigen::Matrix<Scalar, CARTESIAN_DIM + 1, CARTESIAN_DIM + 1> transformationMatrix =
    Eigen::Matrix<Scalar, CARTESIAN_DIM + 1, CARTESIAN_DIM + 1>::Identity();

  if (CARTESIAN_DIM == 2) {
    for (size_t n = 0; n < numberOfPoints; ++n) {
      const Correspondence & correspondence = correspondences[n];
      const PointType & sourcePoint = sourcePoints[correspondence.sourcePointIndex];
      const PointType & targetPoint = targetPoints[correspondence.targetPointIndex];
      const PointType & targetPointNormal = targetPointsNormals[correspondence.targetPointIndex];

      J(n, 0) = targetPointNormal(0);
      J(n, 1) = targetPointNormal(1);
      J(n, 2) = sourcePoint(0) * targetPointNormal(1) - sourcePoint(1) * targetPointNormal(0);
      Y(n) = (targetPoint - sourcePoint).dot(targetPointNormal);
    }

    Eigen::Matrix<Scalar, 3, 1> estimate = leastSquares_.estimateUsingSVD();

    transformationMatrix(0, 1) = -estimate(2);
    transformationMatrix(1, 0) = estimate(2);
    transformationMatrix(0, 2) = estimate(0);
    transformationMatrix(1, 2) = estimate(1);

  } else {
    for (size_t n = 0; n < numberOfPoints; ++n) {
      const Correspondence & correspondence = correspondences[n];
      const PointType & sourcePoint = sourcePoints[correspondence.sourcePointIndex];
      const PointType & targetPoint = targetPoints[correspondence.targetPointIndex];
      const PointType & targetPointNormal = targetPointsNormals[correspondence.targetPointIndex];

      J(n, 0) = targetPointNormal(0);
      J(n, 1) = targetPointNormal(1);
      J(n, 2) = targetPointNormal(2);
      J(n, 3) = sourcePoint(1) * targetPointNormal(2) - sourcePoint(2) * targetPointNormal(1);
      J(n, 4) = sourcePoint(2) * targetPointNormal(0) - sourcePoint(0) * targetPointNormal(2);
      J(n, 5) = sourcePoint(0) * targetPointNormal(1) - sourcePoint(1) * targetPointNormal(0);
      Y(n) = (targetPoint - sourcePoint).dot(targetPointNormal);
    }

    Eigen::Matrix<Scalar, 6, 1> estimate = leastSquares_.estimateUsingSVD();

    transformationMatrix(0, 1) = -estimate(5);
    transformationMatrix(1, 0) = estimate(5);
    transformationMatrix(0, 2) = estimate(4);
    transformationMatrix(2, 0) = -estimate(4);
    transformationMatrix(1, 2) = -estimate(3);
    transformationMatrix(2, 1) = estimate(3);
    transformationMatrix(0, 3) = estimate(0);
    transformationMatrix(1, 3) = estimate(1);
    transformationMatrix(2, 3) = estimate(2);
  }
  return transformationMatrix;
}

//-----------------------------------------------------------------------------
template<class PointType>
typename FindRigidTransformationByLeastSquares<PointType>::TransformationMatrixType
FindRigidTransformationByLeastSquares<PointType>::estimate_(
  const PointSet<PointType> & sourcePoints,
  const PointSet<PointType> & targetPoints,
  const NormalSet<PointType> & targetPointsNormals)
{
  assert(sourcePoints.size() == targetPoints.size());

  // Allocate least squares matrices
  size_t numberOfPoints = sourcePoints.size();
  leastSquares_.setDataSize(numberOfPoints);

  // Compute Jacobian and residuals
  auto & J = leastSquares_.getJ();
  auto & Y = leastSquares_.getY();

  Eigen::Matrix<Scalar, CARTESIAN_DIM + 1, CARTESIAN_DIM + 1> transformationMatrix =
    Eigen::Matrix<Scalar, CARTESIAN_DIM + 1, CARTESIAN_DIM + 1>::Identity();

  if (CARTESIAN_DIM == 2) {
    for (size_t n = 0; n < numberOfPoints; ++n) {
      const PointType & sourcePoint = sourcePoints[n];
      const PointType & targetPoint = targetPoints[n];
      const PointType & targetPointNormal = targetPointsNormals[n];

      J(n, 0) = targetPointNormal(0);
      J(n, 1) = targetPointNormal(1);
      J(n, 2) = sourcePoint(0) * targetPointNormal(1) - sourcePoint(1) * targetPointNormal(0);
      Y(n) = (targetPoint - sourcePoint).dot(targetPointNormal);
    }

    Eigen::Matrix<Scalar, 3, 1> estimate = leastSquares_.estimateUsingSVD();

    transformationMatrix(0, 1) = -estimate(2);
    transformationMatrix(1, 0) = estimate(2);
    transformationMatrix(0, 2) = estimate(0);
    transformationMatrix(1, 2) = estimate(1);

  } else {
    for (size_t n = 0; n < numberOfPoints; ++n) {
      const PointType & sourcePoint = sourcePoints[n];
      const PointType & targetPoint = targetPoints[n];
      const PointType & targetPointNormal = targetPointsNormals[n];

      J(n, 0) = targetPointNormal(0);
      J(n, 1) = targetPointNormal(1);
      J(n, 2) = targetPointNormal(2);
      J(n, 3) = sourcePoint(1) * targetPointNormal(2) - sourcePoint(2) * targetPointNormal(1);
      J(n, 4) = sourcePoint(2) * targetPointNormal(0) - sourcePoint(0) * targetPointNormal(2);
      J(n, 5) = sourcePoint(0) * targetPointNormal(1) - sourcePoint(1) * targetPointNormal(0);
      Y(n) = (targetPoint - sourcePoint).dot(targetPointNormal);
    }

    Eigen::Matrix<Scalar, 6, 1> estimate = leastSquares_.estimateUsingSVD();

    transformationMatrix(0, 1) = -estimate(5);
    transformationMatrix(1, 0) = estimate(5);
    transformationMatrix(0, 2) = estimate(4);
    transformationMatrix(2, 0) = -estimate(4);
    transformationMatrix(1, 2) = -estimate(3);
    transformationMatrix(2, 1) = estimate(3);
    transformationMatrix(0, 3) = estimate(0);
    transformationMatrix(1, 3) = estimate(1);
    transformationMatrix(2, 3) = estimate(2);
  }

  return transformationMatrix;
}

//-----------------------------------------------------------------------------
template<class PointType>
typename FindRigidTransformationByLeastSquares<PointType>::TransformationMatrixType
FindRigidTransformationByLeastSquares<PointType>::find(
  const PointSet<PointType> & sourcePoints,
  const PointSet<PointType> & targetPoints,
  const NormalSet<PointType> & targetPointsNormals,
  const std::vector<Correspondence> & correspondences)
{
  return estimate_(
    sourcePoints,
    targetPoints,
    targetPointsNormals,
    correspondences);
}

//-----------------------------------------------------------------------------
template<class PointType>
typename FindRigidTransformationByLeastSquares<PointType>::TransformationMatrixType
FindRigidTransformationByLeastSquares<PointType>::find(
  const PreconditionedPointSetType & sourcePoints,
  const PreconditionedPointSetType & targetPoints,
  const NormalSet<PointType> & targetPointsNormals,
  const std::vector<Correspondence> & correspondences)
{
  return find(
    sourcePoints.get(),
    targetPoints.get(),
    targetPointsNormals,
    correspondences);
}


//-----------------------------------------------------------------------------
template<class PointType>
typename FindRigidTransformationByLeastSquares<PointType>::TransformationMatrixType
FindRigidTransformationByLeastSquares<PointType>::find(
  const PointSet<PointType> & sourcePoints,
  const PointSet<PointType> & targetPoints,
  const NormalSet<PointType> & targetPointsNormals)
{
  return estimate_(
    sourcePoints,
    targetPoints,
    targetPointsNormals);
}

//-----------------------------------------------------------------------------
template<class PointType>
typename FindRigidTransformationByLeastSquares<PointType>::TransformationMatrixType
FindRigidTransformationByLeastSquares<PointType>::find(
  const PreconditionedPointSetType & sourcePoints,
  const PreconditionedPointSetType & targetPoints,
  const NormalSet<PointType> & targetPointsNormals)
{
  return find(
    sourcePoints.get(),
    targetPoints.get(),
    targetPointsNormals);
}

template class FindRigidTransformationByLeastSquares<Eigen::Vector2f>;
template class FindRigidTransformationByLeastSquares<Eigen::Vector2d>;
template class FindRigidTransformationByLeastSquares<Eigen::Vector3f>;
template class FindRigidTransformationByLeastSquares<Eigen::Vector3d>;

template class FindRigidTransformationByLeastSquares<HomogeneousCoordinates2f>;
template class FindRigidTransformationByLeastSquares<HomogeneousCoordinates2d>;
template class FindRigidTransformationByLeastSquares<HomogeneousCoordinates3f>;
template class FindRigidTransformationByLeastSquares<HomogeneousCoordinates3d>;

}  // namespace core
}  // namespace romea
