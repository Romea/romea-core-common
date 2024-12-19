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


#ifndef ROMEA_CORE_COMMON__TRANSFORM__ESTIMATION__RANSACRIGIDTRANSFORMATIONMODEL_HPP_
#define ROMEA_CORE_COMMON__TRANSFORM__ESTIMATION__RANSACRIGIDTRANSFORMATIONMODEL_HPP_

// std
#include <vector>

// romea
#include "romea_core_common/transform/estimation/RigidTransformationProjection.hpp"
#include "romea_core_common/transform/estimation/FindRigidTransformationByLeastSquares.hpp"
#include "romea_core_common/transform/estimation/FindRigidTransformationBySVD.hpp"

#include "romea_core_common/regression/ransac/Ransac.hpp"
#include "romea_core_common/regression/ransac/RansacRandomCorrespondences.hpp"

namespace romea
{
namespace core
{

template<class PointType>
class RansacRigidTransformationModel : public RansacModel
{
public:
  using Scalar = typename PointType::Scalar;
  static constexpr size_t CARTESIAN_DIM = PointTraits<PointType>::DIM;
  static constexpr size_t POINT_SIZE = PointTraits<PointType>::SIZE;

  using PreconditionedPointSetType = PreconditionedPointSet<PointType>;
  using PointSetPreconditionerType = PointSetPreconditioner<PointType>;
  using TransformationMatrixType = Eigen::Matrix<Scalar, CARTESIAN_DIM + 1, CARTESIAN_DIM + 1>;

public:
  RansacRigidTransformationModel();

  virtual ~RansacRigidTransformationModel() = default;

  explicit RansacRigidTransformationModel(const RansacRigidTransformationModel<PointType> &) =
  delete;

  RansacRigidTransformationModel<PointType> & operator=(
    const RansacRigidTransformationModel<PointType> &) = delete;

public:
  void loadPointSets(
    const PointSet<PointType> * sourcePoints,
    const PointSet<PointType> * targetPoints);

  void loadTargetNormalSet(const NormalSet<PointType> * targetNormals);

  void loadCorrespondences(
    const std::vector<Correspondence> * correspondences,
    const size_t & numberOfSourcePointsInCorrespondences);

public:
  virtual bool draw(const double & modelDeviationError);

  virtual size_t countInliers(const double & modelDeviationError);

  virtual void refine();

  virtual size_t getNumberOfPoints() const;

  virtual size_t getNumberOfPointsToDrawModel() const;

  virtual size_t getMinimalNumberOfInliers() const;

  virtual double getRootMeanSquareError() const;

  const TransformationMatrixType & getTransformation()const;

public:
protected:
  void allocate_(const size_t & numberOfCorrespondences);

  bool check_(
    const PointSet<PointType> & sourcePoints,
    const PointSet<PointType> & targetPoints,
    const std::vector<Correspondence> & sampleCorrespondences,
    const double & modelDeviationError);

  void compute_(
    const PreconditionedPointSetType & sourcePoints,
    const PreconditionedPointSetType & targetPoints,
    const std::vector<Correspondence> & correspondences);

  void setPreconditioner_(
    const PreconditionedPointSetType & preconditionedSourcePoints,
    const PreconditionedPointSetType & preconditionedTargetPoints);

protected:
  const PointSet<PointType> * sourcePoints_;
  const PointSet<PointType> * targetPoints_;
  const NormalSet<PointType> * targetNormals_;
  const std::vector<Correspondence> * correspondences_;
  const std::vector<double> * correspondencesWeights_;

  std::size_t numberOfSourcePointsInCorrespondences_;
  std::vector<Correspondence> sortedCorrespondences_;

  PointSetPreconditionerType sourcePointsPreconditioner_;
  PreconditionedPointSetType precondionedSourcePoints_;
  PointSetPreconditionerType targetPointsPreconditioner_;
  PreconditionedPointSetType precondionedTargetPoints_;

  std::vector<Correspondence> inlierCorrespondences_;
  std::vector<Correspondence> bestInlierCorrespondences_;
  double bestRootMeanSquareError_;

  RansacRandomCorrespondences<PointType> randomCorrespondences_;

  TransformationMatrixType transformation_;
  FindRigidTransformationBySVD<PointType> findRigidTransformationBySVD_;
  FindRigidTransformationByLeastSquares<PointType> findRigidTransformationByLeastSquares_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__TRANSFORM__ESTIMATION__RANSACRIGIDTRANSFORMATIONMODEL_HPP_
