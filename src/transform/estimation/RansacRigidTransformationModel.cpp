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
#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>

// local
#include "romea_core_common/transform/estimation/RansacRigidTransformationModel.hpp"


namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
template<class PointType>
RansacRigidTransformationModel<PointType>::RansacRigidTransformationModel()
: sourcePoints_(nullptr),
  targetPoints_(nullptr),
  targetNormals_(nullptr),
  correspondences_(nullptr),
  correspondencesWeights_(nullptr),
  numberOfSourcePointsInCorrespondences_(0),
  sortedCorrespondences_(),
  sourcePointsPreconditioner_(),
  precondionedSourcePoints_(),
  targetPointsPreconditioner_(),
  precondionedTargetPoints_(),
  inlierCorrespondences_(),
  bestInlierCorrespondences_(),
  bestRootMeanSquareError_(std::numeric_limits<double>::max()),
  randomCorrespondences_(),
  transformation_(),
  findRigidTransformationBySVD_(),
  findRigidTransformationByLeastSquares_()
{
}


// //-----------------------------------------------------------------------------
// template <class PointType> void
// RansacRigidTransformationModel<PointType>::init(
//   const PointSet<PointType> & sourcePoints,
//   const PointSet<PointType> & targetPoints,
//   const PointSet<PointType> & targetNormals,
//   const std::vector<Correspondence> & correspondences,
//   const TransformationMatrixType & guessTransformation)
// {

//  //Create matched point sets
//  size_t numberOfMatchedPoints =std::begin(correspondences_.size();
//  sourcePoints_.resize(numberOfMatchedPoints);
//  targetPoints_.resize(numberOfMatchedPoints);
//  targetNormals_.resize(numberOfMatchedPoints);
//  correspondences_.resize(numberOfMatchedPoints);

//  for(size_t n=0;n<numberOfMatchedPoints;++n){
//    const Correspondence & correspondence = correspondences[n];
//    targetPoints_[n]=targetPoints[correspondence.targetPointIndex];
//    targetNormals_[n]=targetNormals[correspondence.targetPointIndex];
//    projection(guessTransformation,sourcePoints[correspondence.sourcePointIndex],sourcePoints_[n]);
//    correspondences_[n].sourcePointIndex=n;
//    correspondences_[n].targetPointIndex=n;
//  }

// }

//-----------------------------------------------------------------------------
template<class PointType>
void
RansacRigidTransformationModel<PointType>::loadPointSets(
  const PointSet<PointType> * sourcePoints,
  const PointSet<PointType> * targetPoints)
{
  assert(sourcePoints);
  assert(targetPoints);
  assert(sourcePoints->size() == targetPoints->size());

  this->sourcePoints_ = sourcePoints;
  this->targetPoints_ = targetPoints;

  this->sourcePointsPreconditioner_.compute(*sourcePoints);
  this->targetPointsPreconditioner_.compute(*targetPoints);

  Scalar scale = this->targetPointsPreconditioner_.getScale();
  this->precondionedSourcePoints_.compute(*sourcePoints, scale);
  this->precondionedTargetPoints_.compute(*targetPoints, scale);

  this->setPreconditioner_(
    this->precondionedSourcePoints_,
    this->precondionedTargetPoints_);

  this->randomCorrespondences_.computeScale(
    this->sourcePointsPreconditioner_.getPointSetMin(),
    this->sourcePointsPreconditioner_.getPointSetMax());
}


//-----------------------------------------------------------------------------
template<class PointType>
void
RansacRigidTransformationModel<PointType>::loadCorrespondences(
  const std::vector<Correspondence> * correspondences,
  const size_t & numberOfSourcePointsInCorrespondences)
{
  assert(sourcePoints_);
  assert(targetPoints_);

  allocate_(correspondences->size());

  // Store correpondences data
  correspondences_ = correspondences;
  numberOfSourcePointsInCorrespondences_ = numberOfSourcePointsInCorrespondences;

  // Sort correspondences by target index and distance
  sortedCorrespondences_ = *correspondences_;
  std::sort(
    std::begin(sortedCorrespondences_),
    std::end(sortedCorrespondences_),
    sortByTargetIndexAndDistancePredicate);

  // Clear data compute during the last estimation
  bestInlierCorrespondences_.clear();
  bestRootMeanSquareError_ = std::numeric_limits<double>::max();
  assert(bestInlierCorrespondences_.capacity() >= correspondences->size());
}


//-----------------------------------------------------------------------------
template<class PointType>
void
RansacRigidTransformationModel<PointType>::loadTargetNormalSet(
  const NormalSet<PointType> * targetNormals)
{
  if (targetNormals) {
    assert(this->targetPoints_->size() == targetNormals->size());
  }

  targetNormals_ = targetNormals;
}

//-----------------------------------------------------------------------------
template<class PointType>
size_t
RansacRigidTransformationModel<PointType>::getNumberOfPointsToDrawModel()const
{
  if (CARTESIAN_DIM == 2) {
    return 3;
  } else {
    return 4;
  }
}

//-----------------------------------------------------------------------------
template<class PointType>
size_t
RansacRigidTransformationModel<PointType>::getMinimalNumberOfInliers()const
{
  static size_t minimalNumberOfInliers = 2 * getNumberOfPointsToDrawModel();
  return minimalNumberOfInliers;
}

//-----------------------------------------------------------------------------
template<class PointType>
void RansacRigidTransformationModel<PointType>::compute_(
  const PreconditionedPointSetType & sourcePoints,
  const PreconditionedPointSetType & targetPoints,
  const std::vector<Correspondence> & correspondences)
{
  if (targetNormals_) {
    this->transformation_ = findRigidTransformationByLeastSquares_.
      find(sourcePoints, targetPoints, *targetNormals_, correspondences);
  } else {
    this->transformation_ = findRigidTransformationBySVD_.
      find(sourcePoints, targetPoints, correspondences);
  }
}

//-----------------------------------------------------------------------------
template<class PointType>
void RansacRigidTransformationModel<PointType>::setPreconditioner_(
  const PreconditionedPointSetType & preconditionedSourcePoints,
  const PreconditionedPointSetType & preconditionedTargetPoints)
{
  findRigidTransformationByLeastSquares_.
  setPreconditioner(
    preconditionedSourcePoints,
    preconditionedTargetPoints);
}

//-----------------------------------------------------------------------------
template<class PointType>
void RansacRigidTransformationModel<PointType>::allocate_(const size_t & numberOfCorrespondences)
{
  if (bestInlierCorrespondences_.capacity() < numberOfCorrespondences) {
    sortedCorrespondences_.reserve(numberOfCorrespondences);
    bestInlierCorrespondences_.reserve(numberOfCorrespondences);
    inlierCorrespondences_.reserve(numberOfCorrespondences);
  }

  //  sortedCorrespondences_.resize(numberOfCorrespondences);
  //  bestInlierCorrespondences_.resize(numberOfCorrespondences);
  //  inlierCorrespondences_.resize(numberOfCorrespondences);
}

//-----------------------------------------------------------------------------
template<class PointType>
size_t RansacRigidTransformationModel<PointType>::getNumberOfPoints() const
{
  return numberOfSourcePointsInCorrespondences_;
}

//-----------------------------------------------------------------------------
template<class PointType>
bool RansacRigidTransformationModel<PointType>::draw(const double & modelDeviationError)
{
  std::vector<Correspondence> sampleCorrespondences =
    randomCorrespondences_.drawPoints(
    *sourcePoints_, *correspondences_,
    getNumberOfPointsToDrawModel());

  // for (size_t n = 0, N = sampleCorrespondences.size(); n < N; ++n) {
  //   std::cout << sampleCorrespondences[n].sourcePointIndex << " " <<
  //         sampleCorrespondences[n].targetPointIndex << std::endl;
  // }

  compute_(
    precondionedSourcePoints_,
    precondionedTargetPoints_,
    sampleCorrespondences);

  return check_(
    *sourcePoints_,
    *targetPoints_,
    sampleCorrespondences,
    modelDeviationError);
}


//-----------------------------------------------------------------------------
template<class PointType>
size_t RansacRigidTransformationModel<PointType>::countInliers(const double & modelDeviationError)
{
  const PointSet<PointType> & sourcePoints = *sourcePoints_;
  const PointSet<PointType> & targetPoints = *targetPoints_;

  // Find inliers points Indexes
  inlierCorrespondences_.clear();
  assert(inlierCorrespondences_.capacity() >= correspondences_->size());

  PointType projectedSourcePoint;
  Scalar threshold = 9 * modelDeviationError * modelDeviationError;
  for (size_t n = 0, N = sortedCorrespondences_.size(); n < N; n++) {
    const size_t sourcePointIndex = sortedCorrespondences_[n].sourcePointIndex;
    const size_t targetPointIndex = sortedCorrespondences_[n].targetPointIndex;
    projection(transformation_, sourcePoints[sourcePointIndex], projectedSourcePoint);
    Scalar squareError =
      (targetPoints[targetPointIndex] - projectedSourcePoint).array().square().sum();

    if (squareError < threshold) {
      // std::cout << " inlier "
      //           << sourcePointIndex <<" "
      //           << targetPointIndex << " "
      //           << squareError << std::endl;
      inlierCorrespondences_.emplace_back(sourcePointIndex, targetPointIndex, squareError);
    }
  }

  // Compute root mean square error
  std::unique(
    std::begin(inlierCorrespondences_),
    std::end(inlierCorrespondences_),
    equalTargetIndexesPredicate);

  double rootMeanSquareError = 0;
  for (size_t n = 0, N = inlierCorrespondences_.size(); n < N; ++n) {
    rootMeanSquareError += inlierCorrespondences_[n].squareDistanceBetweenPoints;
  }
  rootMeanSquareError /= static_cast<double>(inlierCorrespondences_.size());
  rootMeanSquareError = std::sqrt(rootMeanSquareError);

  // Backup inliers correspondences
  if (inlierCorrespondences_.size() >= getMinimalNumberOfInliers() &&
    rootMeanSquareError < modelDeviationError)
  {
    if (inlierCorrespondences_.size() > bestInlierCorrespondences_.size() ||
      (inlierCorrespondences_.size() == bestInlierCorrespondences_.size() &&
      rootMeanSquareError < bestRootMeanSquareError_))
    {
      bestInlierCorrespondences_ = inlierCorrespondences_;
      bestRootMeanSquareError_ = rootMeanSquareError;
    }
  }
  return bestInlierCorrespondences_.size();
}


//-----------------------------------------------------------------------------
template<class PointType>
void RansacRigidTransformationModel<PointType>::refine()
{
  compute_(
    precondionedSourcePoints_,
    precondionedTargetPoints_,
    bestInlierCorrespondences_);
}

//-----------------------------------------------------------------------------
template<class PointType>
bool
RansacRigidTransformationModel<PointType>::check_(
  const PointSet<PointType> & sourcePoints,
  const PointSet<PointType> & targetPoints,
  const std::vector<Correspondence> & sampleCorrespondences,
  const double & modelDeviationError)
{
  float mse = 0;
  PointType projectedSourcePoint;
  for (size_t n = 0, N = sampleCorrespondences.size(); n < N; ++n) {
    const size_t sourcePointIndex = sampleCorrespondences[n].sourcePointIndex;
    const size_t targetPointIndex = sampleCorrespondences[n].targetPointIndex;
    // transformation_(sourcePoints[sourcePointIndex], projectedSourcePoint);
    projection(transformation_, sourcePoints[sourcePointIndex], projectedSourcePoint);
    mse += (targetPoints[targetPointIndex] - projectedSourcePoint).array().square().sum();
  }

  return (mse / sampleCorrespondences.size()) < modelDeviationError * modelDeviationError;
}

//-----------------------------------------------------------------------------
template<class PointType>
double RansacRigidTransformationModel<PointType>::getRootMeanSquareError() const
{
  return bestRootMeanSquareError_;
}

//-----------------------------------------------------------------------------
template<class PointType>
const typename RansacRigidTransformationModel<PointType>::TransformationMatrixType &
RansacRigidTransformationModel<PointType>::getTransformation()const
{
  return transformation_;
}

template class RansacRigidTransformationModel<Eigen::Vector2f>;
template class RansacRigidTransformationModel<Eigen::Vector2d>;
template class RansacRigidTransformationModel<Eigen::Vector3f>;
template class RansacRigidTransformationModel<Eigen::Vector3d>;

template class RansacRigidTransformationModel<HomogeneousCoordinates2f>;
template class RansacRigidTransformationModel<HomogeneousCoordinates2d>;
template class RansacRigidTransformationModel<HomogeneousCoordinates3f>;
template class RansacRigidTransformationModel<HomogeneousCoordinates3d>;

}  // namespace core
}  // namespace romea
